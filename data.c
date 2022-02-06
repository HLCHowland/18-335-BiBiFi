#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

#define NON_VAR_LENGTH 0     //TODO change me

int compute_record_size(Record *R) {
  int len = 0;
  //TODO do stuff
  
  return len;
}

Buffer print_record(Record *R) {
  Buffer  B = {0};
 
  //TODO Code this

  return B;
}

//produce A | B
Buffer concat_buffs(Buffer *A, Buffer *B) {
  Buffer  C = {0}; 
  //TODO Code this
  return C;
}

void write_to_path(char *path, Buffer *B, unsigned char *key_data) {
  
  //TODO Code this

  return;
}

Buffer read_from_path(char *path, unsigned char *key_data) {
  Buffer  B = {0};
  
  //TODO Code this
  
  return B;
}

void dump_record(Record *R) { 
  
  //TODO Code this
  
  return;
}

int get_record(Record *R, Buffer *B) {
  unsigned int  bytesRead = 0;
 
  //TODO Code this

  return bytesRead;
}

int read_records_from_path(char *path, unsigned char *key, Record **outbuf, unsigned int *outnum) {
  *outnum = 0;
  *outbuf = NULL;

  //read in a file 
  Buffer  B = read_from_path(path, key);

  //TODO Code this
  
  return 0;
}

void serialize_int(char *buf, int a) {
    buf[0] = 0xff & (a >> 24);
    buf[1] = 0xff & (a >> 16);
    buf[2] = 0xff & (a >> 8);
    buf[3] = 0xff & a;
}

int deserialize_int(unsigned char *buf) {
    return ((unsigned char)buf[0]<<24) | ((unsigned char)buf[1]<<16) | ((unsigned char)buf[2]<<8) | (unsigned char)buf[3];
}
