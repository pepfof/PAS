#include <gtk/gtk.h>
#include <stdbool.h>
#include "atq_interface.h"
#include <time.h>

GtkWidget *g_sendnowbutton;
GtkSpinButton *g_hourdial;
GtkSpinButton *g_minutedial;
GtkCalendar *g_calendial;
GtkWidget *g_messagebutton;
GtkEntry *g_messageinput;
GtkWidget *g_mediabutton;
GtkFileChooser *g_fileinput;
GtkWidget *g_marq;
GtkWidget *g_deleb;
GtkWidget *g_bellb;
GtkButton *g_sendb;
GtkButton *g_nowsb;
GtkTreeView *g_list;
GtkTreeSelection *g_selection; 

bool messagein, mediain, sendnow;

bool reportsent;
int reporttimer;

void write_pa(){
    char temp[256]=""; 
    char temptemp[512]="";
    if((messagein and (gtk_entry_get_text (g_messageinput)[0]!=0) or (mediain and gtk_file_chooser_get_filename (g_fileinput)!=NULL))){
    unsigned int day, month, year;
    gtk_calendar_get_date(g_calendial, &year, &month, &day);
    sprintf(temptemp, "%02d_%02d_%04d_%02d_%02d.pa", day, month, year, gtk_spin_button_get_value_as_int (g_hourdial),gtk_spin_button_get_value_as_int (g_minutedial));
    strcat(temp, "./pannouncements/");
    strcat(temp, temptemp);
    /*sprintf(temptemp, "Отослано: %s", temp);
    gtk_label_set_text(g_reportstring, temptemp);*/
    FILE *fp =fopen(temp, "w");
    sprintf(temptemp, "pas-send -I %d -F %d -i \"%s\" -f \"%s\" ", messagein, mediain ,gtk_entry_get_text (g_messageinput),  gtk_file_chooser_get_filename (g_fileinput));
    fprintf(fp,"%s",temptemp);
    reportsent=1;
    sprintf(temptemp, "at %02d:%02d %04d-%02d-%02d -f %s", gtk_spin_button_get_value_as_int (g_hourdial),gtk_spin_button_get_value_as_int (g_minutedial),year ,month+1, day, temp);
    //fprintf(fp,"%s",temptemp);
    fclose(fp);
    delete_entry_at(year, month, day,gtk_spin_button_get_value_as_int (g_hourdial),gtk_spin_button_get_value_as_int (g_minutedial));
    int j = system(temptemp);
    sprintf(temptemp, "rm ./pannouncements/%02d_%02d_%04d_%02d_%02d.pa", day, month, year, gtk_spin_button_get_value_as_int (g_hourdial),gtk_spin_button_get_value_as_int (g_minutedial));
    if(j==256) system(temptemp);
    readatq(g_list);
        }
}

void send_pa(){
    char temp[256]=""; 
    char temptemp[512]="";
    if((messagein and (gtk_entry_get_text (g_messageinput)[0]!=0) or (mediain and gtk_file_chooser_get_filename (g_fileinput)!=NULL))){
    unsigned int day, month, year;
    gtk_calendar_get_date(g_calendial, &year, &month, &day);
    sprintf(temptemp, "now.pa");
    strcat(temp, "./pannouncements/");
    strcat(temp, temptemp);
    /*sprintf(temptemp, "Отослано: %s", temp);
    gtk_label_set_text(g_reportstring, temptemp);*/
    FILE *fp =fopen(temp, "w");
    sprintf(temptemp, "pas-send -I %d -F %d -i \"%s\" -f \"%s\" ", messagein, mediain ,gtk_entry_get_text (g_messageinput),  gtk_file_chooser_get_filename (g_fileinput));
    fprintf(fp,"%s",temptemp);
    reportsent=1;
    sprintf(temptemp, "sh %s", temp);
    fprintf(fp,"%s",temptemp);
    fclose(fp);
    int j = system(temptemp);
        }
}

static void delete_pa(){
        GtkTreeIter iter;
        char temp[256]=""; 
        char temptemp[512]="";
        long long int tempnum = 0;
        GtkTreeSelection *select;
        select = gtk_tree_view_get_selection (GTK_TREE_VIEW (g_list));
        int tdate= 0, tmonth=0, tyear=0, thour=0, tminute=0;
        GtkListStore *store;
        store = GTK_LIST_STORE(gtk_tree_view_get_model
      (GTK_TREE_VIEW(g_list)));

        GtkTreeModel *model=GTK_TREE_MODEL(store);
        if (gtk_tree_selection_get_selected (select, &model, &iter))
        {

            gtk_tree_selection_unselect_iter (select, &iter);
            gtk_tree_model_get (model, &iter, 1, &tempnum, 0, temp, -1);
        gtk_list_store_remove (store, &iter);
    sscanf(temp, "%02d/%02d/%04d %02d:%02d", &tdate, &tmonth, &tyear, &thour, &tminute);
    sprintf(temptemp, "atrm %lld", tempnum);
    system(temptemp);
    }
}

void gtk_greyout(GtkWidget *widgetin, GtkWidget *widgetout, bool invert, bool *output){
    if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widgetin))) {gtk_widget_set_sensitive(GTK_WIDGET(widgetout), invert); *output=invert;}
    else{ gtk_widget_set_sensitive(GTK_WIDGET(widgetout), !invert); *output=!invert;}
}

void gtk_greyout_switch(GtkWidget *widgetin, GtkWidget *widgetout, bool invert,  bool *output){
    if(gtk_switch_get_active (GTK_SWITCH(widgetin))){ gtk_widget_set_sensitive(GTK_WIDGET(widgetout), invert);*output=invert;}
    else {gtk_widget_set_sensitive(GTK_WIDGET(widgetout), !invert); *output=!invert;}
}

void gtk_greyout_opacity(GtkWidget *widgetin, GtkWidget *widgetout, bool invert, bool *output){
    if(gtk_switch_get_active (GTK_SWITCH(widgetin))) {gtk_widget_set_sensitive(GTK_WIDGET(widgetout), invert); gtk_widget_set_opacity(GTK_WIDGET(widgetout), invert ? 1 : 0.5);*output=invert;}
    else{ gtk_widget_set_sensitive(GTK_WIDGET(widgetout), !invert); gtk_widget_set_opacity(GTK_WIDGET(widgetout),invert ? 0.5 : 1);*output=invert;}
}

void quit()
{
    _exit(0);
}

int main(int argc, char *argv[])
{
    long long int lastread=time(0);
    GtkBuilder      *builder; 
    GtkWidget       *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "pas-control.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(G_OBJECT(window),
        "destroy", quit, NULL);
    
    // get pointers all the gtk objects
    g_sendnowbutton = GTK_WIDGET(gtk_builder_get_object(builder, "sendnow"));
    g_hourdial = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "hourdial"));
    g_minutedial = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "minutedial"));
    g_calendial = GTK_CALENDAR(gtk_builder_get_object(builder, "calendial"));
    g_messagebutton = GTK_WIDGET(gtk_builder_get_object(builder, "messagebutton"));
    g_messageinput = GTK_ENTRY(gtk_builder_get_object(builder, "messageinput"));
    g_mediabutton = GTK_WIDGET(gtk_builder_get_object(builder, "mediabutton"));
    g_fileinput = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "fileinput"));
    g_marq = GTK_WIDGET(gtk_builder_get_object(builder, "marq"));
    g_deleb = GTK_WIDGET(gtk_builder_get_object(builder, "deleb"));
    g_sendb = GTK_BUTTON(gtk_builder_get_object(builder, "sendb"));
    g_nowsb = GTK_BUTTON(gtk_builder_get_object(builder, "nowsb"));
    g_list = GTK_TREE_VIEW(gtk_builder_get_object(builder, "entrylist"));
    g_selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(g_list));
    //g_signal_connect(g_selection, "changed", G_CALLBACK(on_changed), label);
    g_object_unref(builder);
     
    g_signal_connect (g_nowsb, "clicked", G_CALLBACK (send_pa), (gpointer) "nowsb");
    g_signal_connect (g_sendb, "clicked", G_CALLBACK (write_pa), (gpointer) "sendb");
    g_signal_connect (g_deleb, "clicked", G_CALLBACK (delete_pa), (gpointer) "deleb");

    gtk_widget_show(window);

    while(1) {
        if(time(0)-lastread>0){
      readatq(g_list);
      lastread=time(0);
      gtk_main_iteration_do(0);}
      gtk_greyout_switch(g_messagebutton, GTK_WIDGET(g_messageinput), 1, &messagein);
      gtk_greyout_switch(g_mediabutton, GTK_WIDGET(g_fileinput), 1, &mediain);
      gtk_main_iteration();
      }

    return 0;
}