#ifndef __TOKEN_2_H__
#define __TOKEN_2_H__

extern bool init_token2(char* ckey, char* tkey);
extern bool encrypt_token2(unsigned int uid, char* token, int* len);
extern bool decrypt_token2(const char* token, int token_len, unsigned int* uid, int* time);


#endif//__TOKEN_2_H__
