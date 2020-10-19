#include <gtk/gtk.h>
#include <stdbool.h>

GtkWidget *g_sendnowbutton;
GtkSpinButton *g_hourdial;
GtkSpinButton *g_minutedial;
GtkSpinButton *g_secondial;
GtkCalendar *g_calendial;
GtkWidget *g_messagebutton;
GtkEntry *g_messageinput;
GtkWidget *g_mediabutton;
GtkFileChooser *g_fileinput;
GtkWidget *g_marq;
GtkWidget *g_defub;
GtkWidget *g_bellb;
GtkWidget *g_sendb;

bool messagein, mediain, sendnow;

bool reportsent;
int reporttimer;
void write_pa(){
    char temp[256]=""; 
    char temptemp[512]="";
    unsigned int day, month, year;
    gtk_calendar_get_date(g_calendial, &year, &month, &day);
    sprintf(temptemp, "%d_%d_%d_%d_%d_%d.pa", day, month, year, gtk_spin_button_get_value_as_int (g_hourdial),gtk_spin_button_get_value_as_int (g_minutedial),gtk_spin_button_get_value_as_int (g_secondial));
    strcat(temp, "./pannouncements/");
    strcat(temp, temptemp);
    /*sprintf(temptemp, "Отослано: %s", temp);
    gtk_label_set_text(g_reportstring, temptemp);*/
    freopen (temp, "w", stdout);
    sprintf(temptemp, "x!; D%d; M%d; Y%d; h%d; m%d; s%d;", day, month, year, gtk_spin_button_get_value_as_int (g_hourdial),gtk_spin_button_get_value_as_int (g_minutedial),gtk_spin_button_get_value_as_int (g_secondial));
    printf("%s",temptemp);
    sprintf(temptemp, " I%d; F%d; i%s; f%s;", messagein, mediain ,gtk_entry_get_text (g_messageinput),  gtk_file_chooser_get_filename (g_fileinput));
    printf("%s",temptemp);
    reportsent=1;
    fclose(stdout);
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
    if(gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widgetin))) {gtk_widget_set_sensitive(GTK_WIDGET(widgetout), invert); gtk_widget_set_opacity(GTK_WIDGET(widgetout), invert ? 1 : 0.5);*output=invert;}
    else{ gtk_widget_set_sensitive(GTK_WIDGET(widgetout), !invert); gtk_widget_set_opacity(GTK_WIDGET(widgetout),invert ? 0.5 : 1);*output=invert;}
}

void quit()
{
    _exit(0);
}

int main(int argc, char *argv[])
{
    GtkBuilder      *builder; 
    GtkWidget       *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "pas.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_builder_connect_signals(builder, NULL);
    g_signal_connect(G_OBJECT(window),
        "destroy", quit, NULL);
    
    // get pointers all the gtk objects
    g_sendnowbutton = GTK_WIDGET(gtk_builder_get_object(builder, "sendnow"));
    g_hourdial = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "hourdial"));
    g_minutedial = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "minutedial"));
    g_secondial = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "secondial"));
    g_calendial = GTK_CALENDAR(gtk_builder_get_object(builder, "calendial"));
    g_messagebutton = GTK_WIDGET(gtk_builder_get_object(builder, "messagebutton"));
    g_messageinput = GTK_ENTRY(gtk_builder_get_object(builder, "messageinput"));
    g_mediabutton = GTK_WIDGET(gtk_builder_get_object(builder, "mediabutton"));
    g_fileinput = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "fileinput"));
    g_marq = GTK_WIDGET(gtk_builder_get_object(builder, "marq"));
    g_defub = GTK_WIDGET(gtk_builder_get_object(builder, "defub"));
    g_sendb = GTK_WIDGET(gtk_builder_get_object(builder, "sendb"));
    g_bellb = GTK_WIDGET(gtk_builder_get_object(builder, "bellb"));
    g_object_unref(builder);

    g_signal_connect (g_sendb, "clicked", G_CALLBACK (write_pa), (gpointer) "sendb");

    gtk_widget_show(window);

    while(1) {
      gtk_greyout(g_sendnowbutton, GTK_WIDGET(g_hourdial), 0, &sendnow);
      gtk_greyout(g_sendnowbutton, GTK_WIDGET(g_minutedial), 0,&sendnow);
      gtk_greyout(g_sendnowbutton, GTK_WIDGET(g_secondial), 0,&sendnow);
      gtk_greyout_opacity(g_sendnowbutton, GTK_WIDGET(g_calendial), 0,&sendnow);
      gtk_greyout_switch(g_messagebutton, GTK_WIDGET(g_messageinput), 1, &messagein);
      gtk_greyout_switch(g_mediabutton, GTK_WIDGET(g_fileinput), 1, &mediain);
      /*if(reportsent){reporttimer=time(0); reportsent=0;}
      if(time(0)-reporttimer>1){gtk_label_set_text(g_reportstring, "Ожидание ввода...");}
      */
      gtk_main_iteration();
      }

    return 0;
}