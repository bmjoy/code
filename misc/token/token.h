#ifndef __TOKEN_CPP_H__
#define __TOKEN_CPP_H__


#define TOKEN_CKEY_LEN			16
#define TOKEN_TKEY_LEN			8
#define TOKEN_NICKNAME_LEN		64

/*
encrypt_token ����Token
����:
uid        �û�ID
pid        ������ƽ̨ID
puid       �������û�ID
nickname   �ǳ�, ���64�ֽ�
token      Token�������
len        Token�����С, ʹ����Ԥ��1K����ֵ
���:
token      ���ܲ���base64encode�����Token
len        ����Token�ĳ���

����ֵ:
����ʧ��   false, δ��ʼ��
���ܳɹ�   true
*/

//extern "C"{

bool encrypt_token(char* ckey, char* tkey, unsigned int uid, unsigned int pid, unsigned int puid, char* nickname, char* token, int* len);


/*
decrypt_token ����Token
����: 
token      ��0��β�ı�׼�ַ���, ��󲻳���1K
���:
uid        �û�ID
pid        ������ƽ̨ID
puid       �������û�ID
nickname   �ǳ�, ���64�ֽ�
time       Token����ʱ��
����ֵ:
���ܳɹ�   ����true
�Ƿ�Token  ����false, �������������ֵ
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
