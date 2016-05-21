#include "aes.h"
#include "base64.h"
#include "token.h"
#include <stdlib.h>
#include <time.h>

//#include <string.h>
//#include <stdio.h>


typedef struct tag_token_item
{
	char                tkey[TOKEN_TKEY_LEN];
	unsigned int        uid;
	unsigned int        pid;
	unsigned int        puid;
	int                 time;
	char                nickname[TOKEN_NICKNAME_LEN];
}token_item;

bool encrypt_token(char* ckey, char* tkey, unsigned int uid, unsigned int pid, unsigned int puid, char* nickname, char* token, int* len)
{
	char buf[1024];
	char buf1[1024];
	int  blen = 0;
	int  i;
    aes_context ctx = { 0 };
    token_item* item = (token_item*)buf1;
    int tlen = 0;
    int clen = 0;
    int m;
    int n;

    if(!token || *len == 0)
        return false;

	memset(buf, 0, sizeof(buf));
	memset(buf1,0, sizeof(buf1));

	if(aes_setkey_enc(&ctx, (unsigned char *)ckey, 128))
		return false;

	memcpy(item->tkey, tkey, 8);
	item->uid = uid;
	item->pid = pid;
	item->puid= puid;
#ifdef _WIN32
	item->time= (int)time(0);

#else
	item->time= time(0);
#endif
	if(strlen(nickname) >= TOKEN_NICKNAME_LEN)
	{
#ifdef _WIN32
#pragma warning(disable:4996)
		strncpy(item->nickname, nickname, TOKEN_NICKNAME_LEN - 1);
	}
	else
	{
        strncpy(item->nickname, nickname, 60);
#pragma warning(default:4996)
#endif
	}

	tlen = sizeof(token_item);
	
	while(blen < tlen)
	{
		int ll = tlen-blen;
		if(ll >15)
			ll = 15;
		memcpy(&buf[clen], &buf1[blen], ll);
		blen += ll;
		clen += ll;
		buf[clen] = rand() % 0xff;
		clen ++;
	}
	m = 16 - (clen % 16);
	for(i=clen; i<clen+m; i++)
		buf[i] = (char)m;
	clen += m;

	n = clen / 16;

	for(i=0; i<n; i++)
	{
		aes_crypt_ecb(&ctx, AES_ENCRYPT, (const unsigned char*)&buf[i*16], (unsigned char*)&buf1[i*16]);
	}

	if(base64_encode((const unsigned char *)buf1, clen, (unsigned char *)token, len))
		return false;
	
	return true;
}

bool decrypt_token(char* ckey, char* tkey, const char* token, int token_len, unsigned int* uid, unsigned int* pid, unsigned int* puid, int* time, char* nickname)
{
	aes_context ctx = { 0 };
	char buf1[1024] = {0};
	char buf[1024]  = {0};
	int  i;
    int  bl;
    int  n;
	int  len;
    int  c;
    token_item* pitem = 0;
    len = token_len;
    int restlen = 0;
	memset(buf1, 0, sizeof(buf1));
	memset(buf,  0, sizeof(buf));
	
    if(token_len <= 0)
        return false;
	if(aes_setkey_dec(&ctx, (unsigned char *)ckey, 128))
		return false;

	if(len > 1000)
		return false;

	bl = 1024;

	if(base64_decode((unsigned char *)token, len, (unsigned char *)buf, &bl))
		return false;
	
	n = bl / 16;

    if(n == 0)
        return false;

	for(i=0; i<n; i++)
		aes_crypt_ecb(&ctx, AES_DECRYPT, (unsigned char*)&buf[i*16], (unsigned char*)&buf1[i*16]);

	c = buf1[bl-1];
	bl -= c;

	for(i=0; i<n-1; i++)
	{
		memcpy(&buf[i*15], &buf1[i*16], 15);
	}
    restlen = (bl%16)-1;
    if(restlen > 0)
	    memcpy(&buf[15*(n-1)], &buf1[16*(n-1)], restlen);
	
	pitem = (token_item*)buf;
	for(i=0; i<8; i++)
		if(tkey[i] != pitem->tkey[i])
			return false;

	*uid = pitem->uid;
	*pid = pitem->pid;
	*puid= pitem->puid;
	*time= pitem->time;
	memcpy(nickname, pitem->nickname, sizeof(pitem->nickname));

	return true;
}
