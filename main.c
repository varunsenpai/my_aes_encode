#define AES256 1
#define CBC 1

#include "aes.h"
#include "base64.h"
#include <stdio.h>

const uint8_t iv[] = {0x72,0xD4,0x0C,0xAA,0x8F,0xFF,0xDE,0x55,0x08,0x5E,0x76,0x47,0x98,0x38,0x63,0x8C};
char *key= "ThisIsKey16bytes";

typedef struct 
{
    uint32_t serial_number;                     /* Serial number of the card*/
    uint32_t tokens_remaining;                  /* Tokens left */
    uint32_t tokens_on_vial;                    /* Tokens on the vial */
    uint32_t original_tokens;                   /* Maximum capacity of tokens */
} rows_t;

typedef struct
{
    uint8_t magic_number[3];
    uint8_t version_number;
    uint32_t serial_number;
    uint32_t token_count;
    uint32_t original_token_count;
} __attribute__((packed)) vial_dets_t;

int main(int argc, char *argv[])
{
    vial_dets_t v1 = {0};

    v1.magic_number[0] = 0x18;
    v1.magic_number[1] = 0x56;
    v1.magic_number[2] = 0xEB;
    v1.version_number = 1;
    v1.serial_number = 0xABCDEF02;
    v1.token_count = 0xFF;
    v1.original_token_count = 0xFF;

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, (uint8_t *)key, iv);
    AES_CBC_encrypt_buffer(&ctx, (uint8_t *)&v1, sizeof(vial_dets_t));
    uint8_t v1_encrypted[16] = { 0} ;
    memcpy(v1_encrypted, &v1, 16);

    for(int i = 0; i < 16; i++)
    {
        printf("%02X", v1_encrypted[i]);
    }
    printf("\n");

    uint8_t v1_updated[16] = { 0 };
    uint8_t v1_encoded[24] = { 0 };
    base64_encode_buffer((uint8_t *)&v1, v1_encoded, sizeof(vial_dets_t), sizeof(v1_encoded));
    for(int i = 0; i < 24; i++)
    {
        printf("%c", v1_encoded[i]);
    }
    printf("\n");

    base64_decode_buffer(v1_encoded, v1_updated, 24, 16);
    AES_ctx_set_iv(&ctx, iv);
    AES_CBC_decrypt_buffer(&ctx, v1_updated, sizeof(vial_dets_t));

    for(int i = 0; i < 16; i++)
    {
        printf("%02X", v1_updated[i]);
    }
    printf("\n");

    return 0;
}