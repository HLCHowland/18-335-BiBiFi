#include <stdio.h>
#include <sodium.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
// #include "fileEncryptionTest.h"

#define CHUNK_SIZE 4096
#define CRYPTO_HEADER_SIZE 24
#define CRYPTO_ABYTE_SIZE 17
#define CRYPTO_FINAL_SIZE 3

// printf("HEADER_BYTES: %i\n", crypto_secretstream_xchacha20poly1305_HEADERBYTES);
// printf("ABYTES: %i\n", crypto_secretstream_xchacha20poly1305_ABYTES);
// printf("TAG_FINAL: %i\n", crypto_secretstream_xchacha20poly1305_TAG_FINAL);



// char* dncrypt_buffer(char *source_buffer, int file_size, const unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES]){

//     char * encrypted_buffer;
//     double total_chunks = 0;
//     unsigned long long out_len;
//     int buffer_offset = 0;
//     total_chunks = (double)file_size/(double)CHUNK_SIZE;


//     // This is done because if total_chunks is a fraction, that means there was an additional chunk
//     if(total_chunks != floor(total_chunks)){
//         total_chunks = floor(total_chunks) + 1;
//     }
    
//     // printf("Total Chunks: %f\n", total_chunks);
//     int encrypted_buffer_size = 0;
//     encrypted_buffer_size = (CRYPTO_HEADER_SIZE + CRYPTO_FINAL_SIZE + (total_chunks * CRYPTO_ABYTE_SIZE) + file_size);
        
//     encrypted_buffer = malloc(encrypted_buffer_size);

//     unsigned char  buf_in[CHUNK_SIZE];
//     // unsigned char  encrypted_buffer[CHUNK_SIZE + crypto_secretstream_xchacha20poly1305_ABYTES];
//     unsigned char  header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
//     crypto_secretstream_xchacha20poly1305_state st;
//     crypto_secretstream_xchacha20poly1305_init_push(&st, header, key);
//     printf("header: %s\n", header);

//     memcpy(&encrypted_buffer[buffer_offset], &header, CRYPTO_HEADER_SIZE + 1);
//     printf("Encrypted Data Stage 1: %s\n", encrypted_buffer);
//     buffer_offset = buffer_offset + CRYPTO_HEADER_SIZE;

//     // Writes encrypted body to the buffer
//     crypto_secretstream_xchacha20poly1305_push(&st, &encrypted_buffer[buffer_offset], &out_len, source_buffer, file_size, NULL, 0, crypto_secretstream_xchacha20poly1305_TAG_FINAL);
//     printf("Encrypted Data Stage 2: %s\n", encrypted_buffer);

// }





char* encrypt_buffer(char *source_buffer, int file_size, const unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES]){

    char * encrypted_buffer;
    double total_chunks = 0;
    unsigned long long out_len;
    int buffer_offset = 0;
    total_chunks = (double)file_size/(double)CHUNK_SIZE;


    // This is done because if total_chunks is a fraction, that means there was an additional chunk
    if(total_chunks != floor(total_chunks)){
        total_chunks = floor(total_chunks) + 1;
    }
    
    // printf("Total Chunks: %f\n", total_chunks);
    int encrypted_buffer_size = 0;
    encrypted_buffer_size = (CRYPTO_HEADER_SIZE + CRYPTO_FINAL_SIZE + (total_chunks * CRYPTO_ABYTE_SIZE) + file_size);
        
    encrypted_buffer = malloc(encrypted_buffer_size);

    unsigned char  buf_in[CHUNK_SIZE];
    // unsigned char  encrypted_buffer[CHUNK_SIZE + crypto_secretstream_xchacha20poly1305_ABYTES];
    unsigned char  header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
    crypto_secretstream_xchacha20poly1305_state st;
    crypto_secretstream_xchacha20poly1305_init_push(&st, header, key);
    printf("header: %s\n", header);

// Yes, the header in the file and the buffer are identical, so this will work
    FILE *fp_t;
    fp_t = fopen("testout", "wb");
    fwrite(header, 1, sizeof header, fp_t);

    memcpy(&encrypted_buffer[buffer_offset], &header, CRYPTO_HEADER_SIZE + 1);
    printf("Encrypted Data Stage 1: %s\n", encrypted_buffer);
    buffer_offset = buffer_offset + CRYPTO_HEADER_SIZE;

    // Writes encrypted body to the buffer
    crypto_secretstream_xchacha20poly1305_push(&st, &encrypted_buffer[buffer_offset], &out_len, source_buffer, file_size, NULL, 0, crypto_secretstream_xchacha20poly1305_TAG_FINAL);
    printf("Encrypted Data Stage 2: %s\n", encrypted_buffer);

    return encrypted_buffer;
}


//   NEED TO DO A MEMSET 0 HERE
char* file_to_buffer(char* source_file, int file_size){
    unsigned char  buf_in[CHUNK_SIZE];
    size_t         read_len;
    int            eof;
    FILE *fp_s;
    int file_offset = 0;
    char *mem_file;

    // printf("Sourcefile: %s\n", source_file);
    // printf("file size: %i", file_size);
    mem_file = malloc(file_size);
    
    // Check for null pointers
    if (!mem_file){
        return 0;
    }

    fp_s = fopen(source_file, "rb");
    
    do {
        read_len = fread(buf_in, 1, sizeof buf_in, fp_s);
        
        memcpy(&mem_file[file_offset], &buf_in, read_len);
        file_offset += read_len;
        eof = feof(fp_s);

    } while (! eof);

    fclose(fp_s);
    return mem_file;
    
}


int main(){

    char * mem_file = NULL;
    char * encrypted_buffer = NULL;
    char * decrypted_buffer = NULL;
    
    char source_file[] = "testFileLarge";

    // unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES];
    unsigned char key[] = "mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm";

    // We need to find the file size on the outside of the function because we will
    // have no idea how long the buffered file is without it since you can't
    // derive that by scanning the malloced area of memory

    FILE *fp_s;
    int file_size = 0;
    fp_s = fopen(source_file, "rb");
    fseek(fp_s, 0L, SEEK_END);
    file_size = ftell(fp_s);

    mem_file = file_to_buffer(source_file, file_size);


    // crypto_secretstream_xchacha20poly1305_keygen(key);
    printf("Key Size %i",crypto_secretstream_xchacha20poly1305_KEYBYTES);
    // key size = 32
    // key = "mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm";
    

    encrypted_buffer = encrypt_buffer(mem_file, file_size, key);
    // decrypted_buffer = decrypt_buffer(encrypted_buffer, key);
    


}


// int encrypt(const char *target_file, const char *source_file,
//         const unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES])
// {
//     unsigned char  buf_in[CHUNK_SIZE];
//     unsigned char  buf_out[CHUNK_SIZE + crypto_secretstream_xchacha20poly1305_ABYTES];
//     unsigned char  header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
//     crypto_secretstream_xchacha20poly1305_state st;
//     FILE          *fp_t, *fp_s;
//     unsigned long long out_len;
//     size_t         rlen;
//     int            eof;
//     unsigned char  tag;

//     fp_s = fopen(source_file, "rb");
//     fp_t = fopen(target_file, "wb");
//     crypto_secretstream_xchacha20poly1305_init_push(&st, header, key);
//     fwrite(header, 1, sizeof header, fp_t);
//     do {
//         rlen = fread(buf_in, 1, sizeof buf_in, fp_s);
//         eof = feof(fp_s);
//         tag = eof ? crypto_secretstream_xchacha20poly1305_TAG_FINAL : 0;
//         crypto_secretstream_xchacha20poly1305_push(&st, buf_out, &out_len, buf_in, rlen,
//                                                    NULL, 0, tag);
//         fwrite(buf_out, 1, (size_t) out_len, fp_t);
//     } while (! eof);
//     fclose(fp_t);
//     fclose(fp_s);
//     return 0;
// }

// int decrypt(const char *target_file, const char *source_file,
//         const unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES])
// {
//     unsigned char  buf_in[CHUNK_SIZE + crypto_secretstream_xchacha20poly1305_ABYTES];
//     unsigned char  buf_out[CHUNK_SIZE];
//     unsigned char  header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
//     crypto_secretstream_xchacha20poly1305_state st;
//     FILE          *fp_t, *fp_s;
//     unsigned long long out_len;
//     size_t         rlen;
//     int            eof;
//     int            ret = -1;
//     unsigned char  tag;

//     fp_s = fopen(source_file, "rb");
//     fp_t = fopen(target_file, "wb");
//     fread(header, 1, sizeof header, fp_s);
//     if (crypto_secretstream_xchacha20poly1305_init_pull(&st, header, key) != 0) {
//         goto ret; /* incomplete header */
//     }
//     do {
//         rlen = fread(buf_in, 1, sizeof buf_in, fp_s);
//         eof = feof(fp_s);
//         if (crypto_secretstream_xchacha20poly1305_pull(&st, buf_out, &out_len, &tag,
//                                                        buf_in, rlen, NULL, 0) != 0) {
//             goto ret; /* corrupted chunk */
//         }
//         if (tag == crypto_secretstream_xchacha20poly1305_TAG_FINAL && ! eof) {
//             goto ret; /* premature end (end of file reached before the end of the stream) */
//         }
//         fwrite(buf_out, 1, (size_t) out_len, fp_t);
//     } while (! eof);

//     ret = 0;
// ret:
//     fclose(fp_t);
//     fclose(fp_s);
//     return ret;
// }

// int
// main(void)
// {
//     unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES];

//     if (sodium_init() != 0) {
//         return 1;
//     }
//     crypto_secretstream_xchacha20poly1305_keygen(key);
//     if (encrypt("/tmp/original", "/tmp/original", key) != 0) {
//         printf("Error\n");
//         return 1;
//     }
//     if (decrypt("/tmp/original", "/tmp/original", key) != 0) {
//         return 1;
//     }
//     // if (decrypt("/tmp/decrypted", "/tmp/encrypted", key) != 0) {
//     //     return 1;
//     // }
//     return 0;
// }