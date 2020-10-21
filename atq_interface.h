#include <stdio.h>
#include <unistd.h>
#include <limits.h>


void add_to_list(GtkTreeView *list, const gchar *numb, const gchar *str, guint64 num) {
    
  GtkListStore *store;
  GtkTreeIter iter;

  store = GTK_LIST_STORE(gtk_tree_view_get_model
      (GTK_TREE_VIEW(list)));

  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, 2, numb, 0, str, 1, num,-1);
}

struct entries {
char weekday[4], month[4];
int date, hour, minute, second, year;
long long int number=0;};


struct entries entry[2048];
struct entries oldentry[2048];

int monthdecode(const char* month){
    if(!strcmp(month, "Jan")) return 0;

    if(!strcmp(month, "Feb")) return 1;

    if(!strcmp(month, "Mar")) return 2;

    if(!strcmp(month, "Apr")) return 3;

    if(!strcmp(month, "May")) return 4;

    if(!strcmp(month, "Jun")) return 5;

    if(!strcmp(month, "Jul")) return 6;

    if(!strcmp(month, "Aug")) return 7;

    if(!strcmp(month, "Sep")) return 8;

    if(!strcmp(month, "Oct")) return 9;

    if(!strcmp(month, "Nov")) return 10;

    if(!strcmp(month, "Dec")) return 11;
    return 0;
}

void remove_deleted_entries(GtkTreeView *list){
        GtkTreeIter iter;
        GtkListStore *store;
        store = GTK_LIST_STORE(gtk_tree_view_get_model
      (GTK_TREE_VIEW(list)));
        GtkTreeModel *model=GTK_TREE_MODEL(store);
        long long int tempnum;
        int valid = 1;
        if(gtk_tree_model_get_iter_first (model, &iter)){
        do{
            bool c = 0;
            gtk_tree_model_get (model, &iter, 1, &tempnum, -1);
            int i = 0;
            while(i<2048){
                if(tempnum==entry[i].number){
            c=1;}
            i++;
            }
    if(!c){
        valid = gtk_list_store_remove (store, &iter);
    }
    else{valid=gtk_tree_model_iter_next (model, &iter);}
   }while(valid);}
}

void readatq(GtkTreeView *list){
    int j =0;
    while(j<2048){
    oldentry[j].number=entry[j].number;
    entry[j].number=0;
    j++;
    }
    j=0;
FILE *fp;
FILE *pa;
int status;
char input[128];
fp = popen("atq", "r");
int i = 0;
char temp[64] = "";
char tempfilename[64] = "";
char tempmessage[64]="";
char tempattachment[64]="";
bool tmessage, tfile;
while (fgets(input, 128, fp) != NULL){
    sscanf(input, "%lld %s %s %d %d:%d:%d %d", &entry[i].number, entry[i].weekday, entry[i].month, &entry[i].date, &entry[i].hour, &entry[i].minute, &entry[i].second, &entry[i].year);
    sprintf(temp, "%02d/%02d/%04d %02d:%02d", entry[i].date, monthdecode(entry[i].month), entry[i].year, entry[i].hour, entry[i].minute);
    sprintf(tempfilename, "./pannouncements/%02d_%02d_%04d_%02d_%02d.pa", entry[i].date, monthdecode(entry[i].month), entry[i].year, entry[i].hour, entry[i].minute);
    pa = fopen(tempfilename, "r");
    fscanf(pa, "pas-send -I %d -F %d -i \"%[^\"]s -f \"%63s\"", &tmessage, &tfile, tempmessage, tempattachment);
    bool c= 0;
    while(j<2048){
        if(entry[i].number==oldentry[j].number){
            c=1;
    }j++;
    }
    if(!c){
        if(tmessage){
    add_to_list(list, tempmessage, temp, entry[i].number);}
    else{
    add_to_list(list, "", temp, entry[i].number);
        }
    }
    i++;
    }
    remove_deleted_entries(list);
    pclose(fp);
}


void delete_entry_at(int eyear, int emonth, int edate, int ehour, int eminute){
FILE *fp;
int status;
struct entries entry[2048];
char input[128];
fp = popen("atq", "r");
int i = 0;
while (fgets(input, 128, fp) != NULL){
    sscanf(input, "%lld %s %s %d %d:%d:%d %d", &entry[i].number, entry[i].weekday, entry[i].month, &entry[i].date, &entry[i].hour, &entry[i].minute, &entry[i].second, &entry[i].year);
    if(entry[i].date==edate && monthdecode(entry[i].month)==emonth && entry[i].year==eyear && entry[i].hour==ehour && entry[i].minute==eminute) {
        char temptemp[64];
        sprintf(temptemp, "atrm %lld", entry[i].number);
        int j = system(temptemp);
        /*sprintf(temptemp, "echo %d", j);
        system(temptemp);*/}
    i++;}
pclose(fp);
}