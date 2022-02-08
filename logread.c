#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include <assert.h>


#include "data.h"

char salt[] = "XXXXXXXXXXXXXXXXXXXX";
char CTUID[] = "XXXXXXXXXXXXXXXXXXXX";

int verbose = 0;

void print_records(Record *records, unsigned int num_records) {
  unsigned int i;

  for(i = 0; i < num_records; i++) {
    dump_record(&records[i]);
    printf("----------------\n");
  }

  return;
}

struct Person {
  //TODO put some other stuff here
  char name[MAX_NAME_LEN];
  bool is_employee;
  int roomnow;
  int roomnumber;
  int roomrecord[MAX_NAME_LEN];
  SLIST_ENTRY(Person)   link;
};

SLIST_HEAD(slisthead, Person);


void leave_action(struct slisthead *head, char *name, bool is_employee, int roomPresent) {
  //is this person already in the gallery? 
  struct Person *current; 
  SLIST_FOREACH(current, head, link)
  {
    if(strcmp(current->name,name)==0){
      if(current->is_employee==is_employee){
        if(current->roomnow!=roomPresent){
          printf("%s can't leave a room that haven't entered",current->name);
          return;
        }
        current->roomnow=-1;
        return;
      }
      else{
        printf("guset type conflict %s",current->name);
        return;
      }
    }
  }
  printf("The person haven't enter gallery yet %s\n",current->name);
  return;
}

void arrive_action(struct slisthead *head, char *name, bool is_employee, int roomPresent) {
  //is this person already in the gallery? 
  struct Person *current; 
  SLIST_FOREACH(current, head, link)
    {
      if(strcmp(current->name,name)==0){
        if(current->is_employee==is_employee){
          if(current->roomnow!=-1){
            printf("Haven't leave room %i\n",current->roomnow);
            return;
          }
          if(roomPresent==-1){
            printf("The person is already in the gallery %s\n",current->name);
            return;
          }
          current->roomnow=roomPresent;
          current->roomrecord[current->roomnumber]=roomPresent;
          current->roomnumber++;
          return;
        }
        else{
          printf("Guest type conflict %s\n",current->name);
          return;
        }
      }
    }
    if(roomPresent!=-1){
        printf("The person haven't enter gallery yet. Can't visit a room%s\n",current->name);
        return;
    }
    current=malloc(sizeof(struct Person));
    SLIST_INSERT_HEAD(head, current, link);
    current->roomnow=roomPresent;
    current->is_employee=is_employee;
    strcpy(current->name,name);
    current->roomnumber=0;
    return;

  return;
}


void execute_records(Record *Rarr, unsigned int num_records, struct slisthead *head) {
  int           i;
  int           firstTime = 0;
  unsigned long Seconds; 

  //TODO Code this

  return;
}

void print_time(Record *Rarr, unsigned int num_records, char *name) {
  unsigned long     start_time;
  unsigned long     elapsed_time = 0;
  int               st = 0;
  int               i;

  //TODO code this

  return;
}

void print_rooms(struct Person *first, struct slisthead head, char *name, bool is_employee) {
  int i;
  SLIST_FOREACH(first, &head, link){
        if(strcmp(first->name,name)==0){
          if(first->is_employee == is_employee){
            for(i = 0; i < first->roomnumber; i++){
              printf("%i", first->roomrecord[i]);
              if(i!=first->roomnumber-1){
                printf(","); 
              }
              else{
                printf("\n");
              }
            }
            return;
          }
          else{
            printf("Guest type doesn't match");
            exit(255);
          } 
        }
        
  }        
  return;
}

void print_summary(struct Person *first, struct slisthead head) {
  int totalroom = 0;
  int roomlist[100];
  int i;
  bool is_newroom = true;
  SLIST_FOREACH(first, &head, link){
    if(first!=head.slh_first){
      printf(",");
    }
    printf("%s", first->name);
    is_newroom = true;
    for(i = 0;i < totalroom; i++){
      if(first->roomnow==roomlist[i]) is_newroom=false;
    }
    if(is_newroom == true){
      roomlist[totalroom] = first->roomnow;
      totalroom++;
    }
  }
  
        

  int roomflag;        
  for(i = 0;i < totalroom; i++) {
    bool firstone = true;
    roomflag = roomlist[i];
    printf("\n%i: ",roomflag);
      SLIST_FOREACH(first, &head, link){
        if(first->roomnow == roomflag){
          if(firstone){
            firstone = false;
          }
          else{
            printf(",");
          }
          printf("%s",first->name);
        }
      }
    printf("\n");
  }            
  return;
}

int main(int argc, char *argv[]) {
  int   opt,len;
  //Store parsing result
  char* token = NULL;
  int token_len_input = 0;
  char* name = NULL;
  int name_len = 0;
  bool print_S = false;
  bool print_R = false;
  bool is_employee = false;
  char* logpath = NULL;
  int logpath_len = 0;

  bool EGchecked = false;

// FIrst step: Parse the commnadline

  while ((opt = getopt(argc, argv, "K:SRE:G:TI")) != -1) {
    switch(opt) {
      case 'K':
        token_len_input = strlen(optarg) + 1;
        token = malloc(token_len_input);
        memcpy(token, optarg, token_len_input);
        break;
      case 'S':
        print_S = true;
        break;

      case 'R':
        print_R = true;
        break;

      case 'T':
        printf("unimplemented");
        exit(255); 
        break;

      case 'I':
        printf("unimplemented");
        exit(255);
        break;

      case 'E':
        //employee name
        name_len = strlen(optarg) + 1;
        name = malloc(name_len);
        memcpy(name, optarg, name_len);
        is_employee = true;
        
        // Error out on duplicate flags
        if (EGchecked == true) {
            printf("invalid input\n");
            exit(255);
        }
        EGchecked = true;
        break;

      case 'G':
        //guest name
        name_len = strlen(optarg) + 1;
        name = malloc(name_len);
        memcpy(name, optarg, name_len);
        is_employee = false;
        // Error out on duplicate flags
        if (EGchecked == true) {
            printf("invalid\n");
            exit(255);
        }
        EGchecked = true;
	    break;
    }
  }

//Read the log file name
  if(optind < argc) {
    logpath_len = strlen(argv[optind]) + 1;
    logpath = malloc(logpath_len);
    memcpy(logpath, argv[optind], logpath_len);
  }
  else {
    printf("Input missing log file.\n");
    exit(255);
  }

//Double check commandline
  if(print_S && print_R){
    printf("Can't execute -S and -R together");
    exit(255);
  }
  if(print_R && !EGchecked){
    printf("Can't find a name");
    exit(255);    
  }  

//Second step: check if token matches the one in existing log
    FILE *log_fp;
    int num_read;
    // Open log read-only
    if( access( logpath, F_OK ) != 0 ){
      printf("Log file doesn't exist.");
      exit(255);
    }
    printf("Opening existing logfile.\n");
    log_fp = fopen(logpath, "r");
    char *buf_r;
    buf_r = malloc(4);
    num_read = fread(buf_r, 1, 4, log_fp);
    assert(num_read==4 && "4 bytes expected for token_len");
    int token_len = deserialize_int(buf_r);
    num_read = fread(buf_r, 1, token_len, log_fp);
    assert(num_read==token_len && "num_read not equal to token_len");
    // Compare tokens
    if (strcmp(buf_r, token) != 0) {
        printf("integrity violation\n");
        exit(255);
    }

    // Third step: Read through the logentry, execute log entry one by one

    // -2=not in gallery, -1=in gallery, 0-1073741823=in room
    struct slisthead head = SLIST_HEAD_INITIALIZER(head);
    int current_location = -2;
    buf_r = realloc(buf_r, 4);
    num_read = fread(buf_r, 1, 4, log_fp);
    int last_ts;
    while (num_read != 0) {
        assert(num_read==4 && "4 bytes expected for entry_len");
        // Deserialize one entry
        int entry_len = deserialize_int(buf_r);
        buf_r = realloc(buf_r, entry_len);
        memset(buf_r, 0, entry_len);
        num_read = fread(buf_r, 1, entry_len, log_fp);
        assert(num_read==entry_len && "num_read not equal to entry_len");
        LogEntry L;
        buf_to_logentry(&L, buf_r, entry_len);
        // Check match and update person's location
        if(L.is_arrival==true){
          arrive_action(&head,L.name,L.is_employee,L.roomID);
          }
        else {
          leave_action(&head,L.name,L.is_employee,L.roomID);
          }
        free(L.name);
        // Read next entry
        buf_r = realloc(buf_r, 4);
        num_read = fread(buf_r, 1, 4, log_fp);
    }
    
    // Forth step: Print the information we want
    struct Person *first;
    fclose(log_fp);
    if(logpath!=NULL){
    if(print_S==true){
      print_summary(first, head);
    }
    if((print_R==true) && (name!=NULL))
      print_rooms(first, head, name, is_employee);
  }
}

