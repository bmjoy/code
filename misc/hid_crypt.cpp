#include "hid_crypt.h"
#include "aes.h"
#include <stdio.h>
#ifdef _WIN32
#pragma warning(disable:4996)
#endif

aes_context hid_ctx_enc = {};
aes_context hid_ctx_dec = {};
int is_hid_crypt_init = 0;

const char* hid_crypt_key = "&2dADjad_0*$2ncm";
const char* hid_code_key = "Adk*^d;~";
#ifdef __cplusplus
extern "C" {
#endif

void init()
{
    if(is_hid_crypt_init)
        return;
    aes_setkey_enc(&hid_ctx_enc, (unsigned char*)hid_crypt_key, 128);
    aes_setkey_dec(&hid_ctx_dec, (unsigned char*)hid_crypt_key, 128);
    is_hid_crypt_init = 1;
}


void hid_encode(char* src, int slen, char* dst, int* dlen)
{
    unsigned char tmp[1024];
    unsigned char encbuf[1024];
	int i,elen,lpad;
    init();
    if(!src || !dst)
    {
        (*dlen) = 0;
        return;
    }
    memcpy(tmp, hid_code_key, 8);
    memcpy(&tmp[8], src, slen);

    elen = slen + 8;
    lpad = 16 - (elen % 16);

    for(i=0; i<lpad; i++)
    {
        tmp[i+elen] = lpad;
    }
    elen += lpad;
    tmp[elen] = 0;
    int m = elen / 16;

    if((*dlen)<=(elen*2+1))
    {
        (*dlen) = 0;
        return;
    }

    for(i=0; i<m; i++)
    {
        aes_crypt_ecb(&hid_ctx_enc, AES_ENCRYPT, &tmp[i*16], &encbuf[i*16]);
    }
    for(i=0; i<elen; i++)
    {
        sprintf(&dst[i*2], "%02x", encbuf[i]);
    }
    (*dlen) = elen * 2;
    dst[(*dlen)] = 0;
}

int hid_decode(char* src, int slen, char* dst, int* dlen)
{
    char tmp[1024];
    char decbuf[1024];
	int  m = 0;
	int  i;
    init();

	if(!src || !dst)
        return 0;
    if((slen%16) != 0 || slen > 128)
        return 0;
    int len = slen / 2;
    if(len < 16)
        return 0;

    for(i=0; i<len; i++)
        sscanf(&src[i*2], "%02x", (unsigned char*)&tmp[i]);

    m = len / 16;

    for(i=0; i<m; i++)
        aes_crypt_ecb(&hid_ctx_dec, AES_DECRYPT, (unsigned char*)&tmp[i*16], (unsigned char*)&decbuf[i*16]);

    if(decbuf[len-1]<=0 || decbuf[len-1]>16)
        return 0;
    len -= decbuf[len - 1];
    if(len < 8)
        return 0;

    decbuf[len] = 0;

    for(i=0; i<8; i++)
    {
        if(decbuf[i] != hid_code_key[i])
            return 0;
    }

    if((*dlen) < (len - 8))
        return 0;
    (*dlen) = len - 8;
    memcpy(dst, &decbuf[8], (*dlen));
    dst[(*dlen)] = 0;
    return 1;
}

void test_hid_crypt_(int l)
{
    char src[1024] = {0};
    char dst[1024] = {0};
    int dlen = 1024;
    int slen = 1024;
	int i;
    //模拟数据
    for(i=0; i<l; i++)
        src[i] = 'a';
    printf("原始数据：%s\r\n", src);
    hid_encode(src, l, dst, &dlen);
    printf("加密数据：%s\r\n", dst);
    hid_decode(dst, dlen, src, &slen);
    printf("解密数据：%s\r\n\r\n\r\n", src);
}

void test_hid_crypt()
{
	int i;
    for(i=0; i<64; i++)
        test_hid_crypt_(i);
}

#ifdef __cplusplus
}
#endif
