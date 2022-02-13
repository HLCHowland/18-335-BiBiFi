#include <stdio.h>
#include <sodium.h>
// #include "fileEncryptionTest.h"

#define CHUNK_SIZE 4096


char* file_to_buffer(char* source_file){
    unsigned char  buf_in[CHUNK_SIZE];
    FILE          *fp_t, *fp_s;
    size_t         read_len;
    int            eof;
    int file_size;
    int file_offset = 0;

    char *mem_file;

    printf("Sourcefile: %s\n", source_file);

    fp_s = fopen("script", "rb");
    fseek(fp_s, 0L, SEEK_END);
    file_size = ftell(fp_s);
    printf("file size: %i", file_size);
    fclose(fp_s);
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
    char source_file[] = "script";
    mem_file = file_to_buffer(source_file);
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