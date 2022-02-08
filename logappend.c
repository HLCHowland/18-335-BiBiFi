#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wordexp.h>
#include <stdbool.h>
#include <err.h>
#include <errno.h>
#include <assert.h>

#include "data.h"

char salt[] = "XXXXXXXXXXXXXXXXXXXX";
char CTUID[] = "XXXXXXXXXXXXXXXXXXXX";

typedef struct _CmdLineResult {
  // int     good; Not using good because we exit whenever input is erroneous
  int     ts;
  char*   token;
  int     token_len;
  bool    is_employee;
  bool    is_arrival;
  char*   name;
  int     name_len;
  int     roomID;
  char*   logpath;
  int     logpath_len;
} CmdLineResult;


int do_batch(char *);

// TODO: Is the use of strlen to determine token_len/name_len secure?
CmdLineResult parse_cmdline(int argc, char *argv[], int is_batch) {
  CmdLineResult R = { 0, NULL, 0, false, false, NULL, 0, -1};
  int opt,r = -1;
  int ts = -1;
  bool EGchecked = false;
  bool ALchecked = false;

  //argument data
  // Record  Rec = {0};
  char    *logpath = NULL;
  char    *batchfile = NULL;
  char    *token;
  char    *name;

  //pick up the switches
  while ((opt = getopt(argc, argv, "T:K:E:G:ALR:B:")) != -1) {
    int     br = 0;
    size_t  len;
    
    //TODO Code this
    
    switch(opt) {
      case 'B':
        batchfile = optarg;
        //batch file
	    break;  

      case 'T':
	    //timestamp
	    R.ts = atoi(optarg);
        break;

      case 'K':
	    //secret token
	    R.token_len = strlen(optarg) + 1;
        token = malloc(R.token_len);
        memcpy(token, optarg, R.token_len);
        R.token = token;
        break;

      case 'A':
        //arrival
        R.is_arrival = true;

        // Error out on duplicate flags
        if (ALchecked == true) {
            printf("invalid\n");
            exit(255);
        }
        ALchecked = true;
        break; 
      
      case 'L':
        //departure
        R.is_arrival = false;

        // Error out on duplicate flags
        if (ALchecked == true) {
            printf("invalid\n");
            exit(255);
        }
        ALchecked = true;
        break;

      case 'E':
        //employee name
        R.name_len = strlen(optarg) + 1;
        name = malloc(R.name_len);
        memcpy(name, optarg, R.name_len);
        R.name = name;
        R.is_employee = true;
        
        // Error out on duplicate flags
        if (EGchecked == true) {
            printf("invalid\n");
            exit(255);
        }
        EGchecked = true;
        break;

      case 'G':
        //guest name
        R.name_len = strlen(optarg) + 1;
        name = malloc(R.name_len);
        memcpy(name, optarg, R.name_len);
        R.name = name;
        R.is_employee = false;
        
        // Error out on duplicate flags
        if (EGchecked == true) {
            printf("invalid\n");
            exit(255);
        }
        EGchecked = true;
	    break;

      case 'R':
        //room ID
        R.roomID = atoi(optarg);
        break;

      default:
        //unknown option, leave
        break;
    }

  }
  
  //pick up the positional argument for log path
  if(optind < argc) {
    R.logpath_len = strlen(argv[optind]) + 1;
    logpath = malloc(R.logpath_len);
    memcpy(logpath, argv[optind], R.logpath_len);
    R.logpath = logpath;
  }
  
  //TODO: implement batching

  //if(batchfile != NULL) {
  //  R.good = do_batch(batchfile);
  //} else {
  //  //TODO do stuff
  //  R.good = 0;
  //}

  return R;
}

int do_batch(char *filepath) {
  //read in data from filepath
  char  *data = NULL;
  int   fd = open(filepath, O_RDONLY);

  if(fd <= 0) {
    return -1;
  }

  //TODO Code this

  close(fd);

  return 0;
}

void print_cmdline(CmdLineResult R){
    printf("ts: %d\n", R.ts);
    printf("token: %s\n", R.token);
    printf("token_len: %d\n", R.token_len);
    printf("is_employee: %d\n", R.is_employee);
    printf("is_arrival: %d\n", R.is_arrival);
    printf("name: %s\n", R.name);
    printf("name_len: %d\n", R.name_len);
    printf("roomID: %d\n", R.roomID);
    printf("logpath: %s\n", R.logpath);
    printf("logpath_len: %d\n", R.logpath_len);
}

void print_logentry(LogEntry L){
    printf("ts: %d\n", L.ts);
    printf("is_employee: %d\n", L.is_employee);
    printf("is_arrival: %d\n", L.is_arrival);
    printf("name: %s\n", L.name);
    printf("name_len: %d\n", L.name_len);
    printf("roomID: %d\n", L.roomID);
}


int main(int argc, char *argv[]) {
    int r;
    CmdLineResult R;
    FILE *log_fp;
    int i;
    int num_read;
  
    R = parse_cmdline(argc, argv, 0);
    //print_cmdline(R);

    // First step: check if log file exists.

    // If log file doesn't exist, create a new file, write token, and write in CmdLineResult
    if( access( R.logpath, F_OK ) != 0 ){
        // Create new log
        printf("Log file doesn't currently exists. Opening new log file.\n");
        // Verify first entry is an arrival to the gallery
        if (R.is_arrival==false || R.roomID!=-1) {
            printf("invalid\n");
            printf("invalid first entry.\n");
            exit(255);
        }
        log_fp = fopen(R.logpath, "w+");
        // Write token to beginning of file
        // token_len(int)+token(str)
        char token_len_str[4];
        serialize_int(token_len_str, R.token_len);
        fwrite(token_len_str, 1, 4, log_fp);
        fwrite(R.token, 1, R.token_len, log_fp);

        // Write new command
        char *buf;
        int buf_len;
        LogEntry L;
        L.ts = R.ts;
        L.name = R.name;
        L.name_len = R.name_len;
        L.is_employee = R.is_employee;
        L.is_arrival = R.is_arrival;
        L.roomID = R.roomID;
        buf_len = logentry_to_buf(L, &buf);
        //printf("Printing buf, length of %d:\n", buf_len);
        //for (i=0; i<buf_len; i++) {
        //    printf("%d ",buf[i]);
        //}
        //printf("\n");
        fwrite(buf, 1, buf_len, log_fp);
        fclose(log_fp);
        free(buf);
        return 0;
    }

    // Second step: check if token matches the one in existing log

    // Open log read-only
    printf("Opening existing logfile.\n");
    log_fp = fopen(R.logpath, "r");
    char *buf_r;
    buf_r = malloc(4);
    num_read = fread(buf_r, 1, 4, log_fp);
    assert(num_read==4 && "4 bytes expected for token_len");
    //printf("Printing buf_r, length of %d:\n", 4);
    //for (i=0; i<4; i++) {
    //    printf("%d ",buf_r[i]);
    //}
    //printf("\n");
    int token_len = deserialize_int(buf_r);
    //buf_r = realloc(buf_r, token_len);
    //memset(buf_r, 0, token_len);
    //printf("token_len is %d\n", token_len);
    num_read = fread(buf_r, 1, token_len, log_fp);
    assert(num_read==token_len && "num_read not equal to token_len");
    // Compare tokens
    if (strcmp(buf_r, R.token) != 0) {
        printf("invalid\n");
        printf("token mismatched. %s from input, %s expected from log.\n", R.token, buf_r);
        exit(255);
    }

    // Third step: Read through the log, search for relevant key (name+E/G), and get person's current location

    // -2=not in gallery, -1=in gallery, 0-1073741823=in room
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
        //print_logentry(L);
        // Check match and update person's location
        if ((strcmp(R.name, L.name) == 0) && (R.is_employee == L.is_employee)) {
            //printf("Found matching record for %s. Updating his location.\n", R.name);
            if (L.is_arrival) {
                // Arriving in either gallery or room
                current_location = L.roomID;
            } else {
                if (L.roomID==-1) {
                    // Leaving gallery
                    current_location = -2;
                } else {
                    // Leaving room
                    current_location = -1;
                }
            }
        }
        // Update latest timestamp
        last_ts = L.ts;
        free(L.name);
        // Read next entry
        buf_r = realloc(buf_r, 4);
        num_read = fread(buf_r, 1, 4, log_fp);
    }

    // Fourth step: check if command line input is valid given person's current location
    // Also check for timestamp validity
    printf("%s's current location is: %d\n", R.name, current_location);
    if (R.is_arrival) {
        if (R.roomID==-1 && current_location!=-2) {
            // Person already in gallery entering again
            printf("invalid\n");
            printf("Person already in gallery.\n");
            exit(255);
        }
        if (R.roomID>=0 && current_location!=-1) {
            // Person not in gallery lobby entering room
            printf("invalid\n");
            printf("Person not in gallery lobby.\n");
            exit(255);
        }
    } else {
        if (R.roomID != current_location) {
            // Person leaving not current location
            printf("invalid\n");
            printf("Person not leaving current location.\n");
            exit(255);
        }
    }
    if (R.ts < last_ts) {
        printf("invalid\n");
        printf("Timestamp lower than latest in log.\n");
        exit(255);
    }
    fclose(log_fp);

    // Final step: add command line as a new log entry
    log_fp = fopen(R.logpath, "a");
    char *buf;
    int buf_len;
    LogEntry L;
    L.ts = R.ts;
    L.name = R.name;
    L.name_len = R.name_len;
    L.is_employee = R.is_employee;
    L.is_arrival = R.is_arrival;
    L.roomID = R.roomID;
    buf_len = logentry_to_buf(L, &buf);
    fwrite(buf, 1, buf_len, log_fp);
    fclose(log_fp);
    free(buf);
    
        

  //if(R.good == 0) {
    //Buffer  B = read_from_path(/** stuff **/);

    //TODO do things here.
    
    //write the result back out to the file
    //write_to_path(/** stuff **/);
  //}

  return 0;
}
