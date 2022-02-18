#include <stdio.h>
#include <sodium.h>

#define CHUNK_SIZE 4096
#define CRYPTO_HEADER_SIZE 24
#define CRYPTO_ABYTE_SIZE 17
#define CRYPTO_FINAL_SIZE 3

static int inplace_encrypt(const char *source_file,const unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES]);
static int inplace_decrypt(const char *source_file,const unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES]);

