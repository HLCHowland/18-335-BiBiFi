#include <stdio.h>
#include <sodium.h>
// #include "fileEncryptionTest.c"

#define CHUNK_SIZE 4096

int encrypt(const char *target_file, const char *source_file,const unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES]);
int decrypt(const char *target_file, const char *source_file,const unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES]);

