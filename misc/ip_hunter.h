#ifndef __IP_HUNTER_H__
#define __IP_HUNTER_H__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
���ܣ� IP ��ַ��ѯ
auth�� msl
*/
//����ʹ�ã���ȡ��־�ļ�
extern int read_line(FILE* f, char* buf, int buflen);

//////////////////////////////////////////////////////////////////////////
/*
��ʼ��IP�⡣
ip���ļ���ʽ����Ϊ
IP_FROM \t IP_TO \t ʡ \t �� \t �� \t ����
*/
extern bool init_ip_hunter(const char* filename);
extern void fini_ip_hunter();
/*
����IP������
site1: ʡ
site2: ��
site3: ��/��
site4: ����
����ҵ�����true, ���򷵻�false
*/
extern bool hunter_ip_site(const char* ip,  char** site1, char** site2, char** site3, char** site4, int* sitecode);

#endif//__IP_HUNTER_H__