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
