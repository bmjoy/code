#include "token2.h"
#include <time.h>
#include "aes.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#pragma warning(disable:4996)
#endif

struct token2_data
{
    unsigned int uid;
    int timestamp;
    unsigned char tkey[8];
};

unsigned char token_ckey[32] = {0};
unsigned char token_tkey[32] = {0};
bool is_token_init = false;
aes_context ctx_enc = { 0 };
aes_context ctx_dec = { 0 };


bool init_token2(char* ckey, char* tkey)
{
    memcpy(token_tkey, tkey, 8);
    memcpy(token_ckey, ckey, 16);

    if(aes_setkey_enc(&ctx_enc, token_ckey, 128))
        return false;
    if(aes_setkey_dec(&ctx_dec, token_ckey, 128))
        return false;

    is_token_init = true;
    return true;
}

bool encrypt_token2(unsigned int uid, char* token, int* len)
{
    unsigned char buftok[20] = {0};
    token2_data data;
    if(!is_token_init)
        return false;

    if(*len < 33)
        return false;
    data.uid = uid;
    data.timestamp = _time32(0);
    memcpy(data.tkey, token_tkey, 8);
    data.tkey[7] = rand() % 256;


    aes_crypt_ecb(&ctx_enc, AES_ENCRYPT, (unsigned char*)&data, buftok);
    
    for(int i=0; i<16; i++)
    {
        sprintf(&token[i*2], "%02x", buftok[i]);
    }
    *len = 32;

    return true;
}

bool decrypt_token2(const char* token, int token_len, unsigned int* uid, int* time)
{
    unsigned char buf[20] = {0};
    token2_data data;

    if(!is_token_init)
        return false;
    if(!token || token_len != 32)
        return false;
    for(int i=0; i<16; i++)
        sscanf(&token[i*2], "%02x", &buf[i]);

    aes_crypt_ecb(&ctx_dec, AES_DECRYPT, buf, (unsigned char*)&data);

    for(int i=0; i<7; i++)
    {
        if(data.tkey[i] != token_tkey[i])
            return false;
    }
    *uid = data.uid;
    *time= data.timestamp;

    return true;
}

