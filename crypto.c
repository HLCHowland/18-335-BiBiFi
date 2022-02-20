#include <stdio.h>
#include <sodium.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "crypto.h"

int encrypt( char *source_file,
         char key[crypto_secretstream_xchacha20poly1305_KEYBYTES])
{
    unsigned char  buf_in[CHUNK_SIZE];
    unsigned char  buf_out[CHUNK_SIZE + crypto_secretstream_xchacha20poly1305_ABYTES];
    unsigned char  header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
    crypto_secretstream_xchacha20poly1305_state st;
    FILE          *fp_s;
    unsigned long long out_len;
    size_t         rlen;
    int            eof;
    unsigned char  tag;
    int file_offset = 0;
    int file_size = 0;
    char *mem_file;
    double total_chunks = 0;

    printf("enc 0\n");
    // Gets file size and crypto chunks for buff size calc
    fp_s = fopen(source_file, "rb");
    fseek(fp_s, 0L, SEEK_END);
    file_size = ftell(fp_s);
    fclose(fp_s);

    total_chunks = (double)file_size/(double)CHUNK_SIZE;

    // If total_chunks is a fraction, that means we will need to account for an additional chunk
    if(total_chunks != floor(total_chunks)){
        total_chunks = floor(total_chunks) + 1;
    }
    
    // Decrypted buff size calc
    int encrypted_buffer_size = 0;
    encrypted_buffer_size = (CRYPTO_HEADER_SIZE + (total_chunks * CRYPTO_ABYTE_SIZE) + file_size);

    // Add null byte to end of string
/////////////////////////////////////////////////////////////////////////////////////////////
    mem_file = malloc(encrypted_buffer_size + 1);
//////////////////////////////////////////////////////////////////////////////////////
    // memset( mem_file, '\0', encrypted_buffer_size + 1);
/////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("enc 1\n");

    fp_s = fopen(source_file, "rb");
    crypto_secretstream_xchacha20poly1305_init_push(&st, header, (unsigned char *)key);
    printf("enc 2\n");

    // Write header into the buffer, increment offset
    memcpy(&mem_file[file_offset], &header, sizeof header);
    file_offset = sizeof header;
    printf("enc 3\n");

    // Read file, encrypt, add to buffer
    do {
        rlen = fread(buf_in, 1, sizeof buf_in, fp_s);
        eof = feof(fp_s);
        tag = eof ? crypto_secretstream_xchacha20poly1305_TAG_FINAL : 0;
        if (crypto_secretstream_xchacha20poly1305_push(&st, buf_out, &out_len, buf_in, rlen,
                                                   NULL, 0, tag) != 0){
            printf("Corrupted chunk, exiting.\n");
            exit(-1);
        }

        memcpy(&mem_file[file_offset], &buf_out, (size_t) out_len);
        file_offset = file_offset + (size_t) out_len;
        
    } while (! eof);
    fclose(fp_s);
    printf("enc 4\n");

    // Overwrite the source file with the encrypted data from the buffer
    fp_s = fopen(source_file, "wb");
    fwrite(mem_file, 1, (size_t) encrypted_buffer_size, fp_s);
    free(mem_file);
    fclose(fp_s);
    printf("enc 5\n");

    return 0;
}

int decrypt(char *source_file,
         char key[crypto_secretstream_xchacha20poly1305_KEYBYTES])
{
    unsigned char  buf_in[CHUNK_SIZE + crypto_secretstream_xchacha20poly1305_ABYTES];
    unsigned char  buf_out[CHUNK_SIZE];
    unsigned char  header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
    crypto_secretstream_xchacha20poly1305_state st;
    FILE          *fp_s;
    unsigned long long out_len;
    size_t         rlen;
    int            eof;
    unsigned char  tag;
    int file_offset = 0;
    int file_size = 0;
    char *mem_file;
    double total_chunks = 0;

    printf("enc 0\n");
    // Gets file size and crypto chunks for buff size calc
    fp_s = fopen(source_file, "rb");
    fseek(fp_s, 0L, SEEK_END);
    file_size = ftell(fp_s);
    fclose(fp_s);    
    total_chunks = (double)file_size/(double)CHUNK_SIZE;

    // If total_chunks is a fraction, that means we will need to account for an additional chunk
    if(total_chunks != floor(total_chunks)){
        total_chunks = floor(total_chunks) + 1;
    }

    // Decrypted buff size calc
    int decrypted_buffer_size = 0;
    // decrypted_buffer_size = ((file_size) - (total_chunks * CRYPTO_ABYTE_SIZE) - CRYPTO_HEADER_SIZE);
    // printf("\nDecrypted Buff Vars\nfile_size: %i\ncrypto data size: %f\n     total_chunks: %f\n     ABYTE: %d\ncrypto header size: %d\n", file_size, (total_chunks * CRYPTO_ABYTE_SIZE),total_chunks, CRYPTO_ABYTE_SIZE, CRYPTO_HEADER_SIZE);
    printf("\nDecrypted Buff Vars\nfile_size: %i\ncrypto data size: %i\n     total_chunks: %f\n     ABYTE: %d\ncrypto header size: %d\n", file_size, (1 * CRYPTO_ABYTE_SIZE),total_chunks, CRYPTO_ABYTE_SIZE, CRYPTO_HEADER_SIZE);
    decrypted_buffer_size = ((file_size) - (1 * CRYPTO_ABYTE_SIZE) - CRYPTO_HEADER_SIZE);


    // Add null byte to end of string
    /////////////////////////////////////////////////////////////////////////////////////////
    // decrypted_buffer_size is getting fucked up 
    mem_file = malloc(decrypted_buffer_size + 1);
    // memset( mem_file, '\0', decrypted_buffer_size + 1);
    printf("dec 1 size: %i\n", decrypted_buffer_size + 1);


    fp_s = fopen(source_file, "rb");    
    fread(header, 1, sizeof header, fp_s);
    if (crypto_secretstream_xchacha20poly1305_init_pull(&st, header, (unsigned char *)key) != 0) {
        printf("File .\n");
        exit(-1);
    }
    printf("dec 2\n");
    
    // Read file, decrypt, add to buffer
    do {
        printf("dec 2.1\n");
        rlen = fread(buf_in, 1, sizeof buf_in, fp_s);
        eof = feof(fp_s);
        printf("dec 2.2\n");
        if (crypto_secretstream_xchacha20poly1305_pull(&st, buf_out, &out_len, &tag,
                                                       buf_in, rlen, NULL, 0) != 0) {
            printf("Corrupted chunk, exiting.\n");
            exit(-1);
        }
        printf("dec 2.3\n");
            
        if (tag == crypto_secretstream_xchacha20poly1305_TAG_FINAL && ! eof) {
            printf("Premature end. I get that a lot :(\n");
            exit(-1);
        }

        memcpy(&mem_file[file_offset], &buf_out, (size_t) out_len);
        printf("dec 2.5 outlen %llu offset %i\n", out_len, file_offset);
        
        file_offset = file_offset + (size_t) out_len + 1;
        printf("dec 2.7\n");
            

    } while (! eof);
    printf("dec 2.7\n");
    // fclose(fp_s);
    printf("dec 3\n");

    // Overwrite the source file with the decrypted data from the buffer
    fp_s = fopen(source_file, "wb");
    fwrite(mem_file, 1, (size_t) decrypted_buffer_size, fp_s);
    free(mem_file);
    fclose(fp_s);
    printf("dec 4\n");

    return 0;
}

// int main(void)
// {
//     // unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES];
//     if (sodium_init() != 0) {
//         return 1;
//     }
//     // crypto_secretstream_xchacha20poly1305_keygen(key);
//     unsigned char key[] = "mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm";
//     if (encrypt("./testFileSmall", key) != 0) {
//         return 1;
//     }
//     if (decrypt( "./testFileSmall", key) != 0) {
//         return 1;
//     }
//     return 0;
// }

