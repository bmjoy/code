#ifndef __TOKEN_CPP_H__
#define __TOKEN_CPP_H__


#define TOKEN_CKEY_LEN			16
#define TOKEN_TKEY_LEN			8
#define TOKEN_NICKNAME_LEN		64

/*
encrypt_token 加密Token
输入:
uid        用户ID
pid        第三方平台ID
puid       第三方用户ID
nickname   昵称, 最大64字节
token      Token输出缓存
len        Token缓存大小, 使用是预留1K经验值
输出:
token      加密并做base64encode处理的Token
len        返回Token的长度

返回值:
加密失败   false, 未初始化
加密成功   true
*/

//extern "C"{

bool encrypt_token(char* ckey, char* tkey, unsigned int uid, unsigned int pid, unsigned int puid, char* nickname, char* token, int* len);


/*
decrypt_token 解密Token
输入: 
token      以0结尾的标准字符串, 最大不超过1K
输出:
uid        用户ID
pid        第三方平台ID
puid       第三方用户ID
nickname   昵称, 最大64字节
time       Token加密时间
返回值:
解密成功   返回true
非法Token  返回false, 并忽略所有输出值
*/

bool decrypt_token(char* ckey, char* tkey, const char* token, int token_len, unsigned int* uid, unsigned int* pid, unsigned int* puid, int* time, char* nickname);
#define CHECK_TOKEN(result,_ckey,_tkey,_token,_token_len,uid,pid,puid,time,nickname)\
    bool result;\
    unsigned int uid;\
    unsigned int pid;\
    unsigned int puid;\
    int time;\
    char nickname[64];\
    result = decrypt_token(_ckey,_tkey,_token,_token_len,&uid,&pid,&puid,&time,nickname);


//};
#endif//__TOKEN_CPP_H__
