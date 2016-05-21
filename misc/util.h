#ifndef __LOBBY_SERVER_UTIL_H__
#define __LOBBY_SERVER_UTIL_H__


//清除多余的换行符,空格,制表符
extern int shrink_xml(char* xml, int len);
extern const char* get_mac_address();

#ifdef _DEBUG
#define ODBG printf
#else
#define ODBG 
#endif

extern char* strtok_c(char* src, char d, char** context);
extern char* strtok_w(char* src, short d, char** context);
extern __int64 time_stoi(const char* st);
extern void anti_sql_invade(char* s, int len, char dangerous, char safe);
extern int url_encode(const char* src, char* buf, int buflen);
extern int url_decode(const char* src, char* buf, int buflen);
extern void time_itos(__int64 t, char* out);
extern unsigned int crc32_file(const char* filename);
extern bool check_valid_phone(char* buf, int len);

#endif//__LOBBY_SERVER_UTIL_H__
