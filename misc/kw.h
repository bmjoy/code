#ifndef __KEY_WORDS_TOOL_H__
#define __KEY_WORDS_TOOL_H__

/*
�Ż��ؼ��ֿ��ļ�
���ֿ��ļ��������أ�����˳����߼����ٶ�������ٶȡ�
�ɹ�����0
*/
extern int kw_optimize(const char* srcfilename, const char* outfile);


/*
���س�ʼ���ʿ�
�ɹ�����0
*/
extern int kw_init(const char* kw_file);


/*
����Ƿ���ڹؼ���
����ֵ��0  �޹ؼ��ֳ�ͻ
        -1 ���ڹؼ��ֳ�ͻ
        hint: ��ͻ�Ĺؼ���
*/
extern int kw_check(const char* word, char* hint, int* hintlen);


/*
��г�ַ���
��src�г��ֹؼ��ֵ��滻��**��ÿ���ؼ����滻����**��
����ֵ��0   �޹ؼ��ֳ�ͻ
        -1  ���ڹؼ��ֳ�ͻ��
*/
extern int kw_hexie(char* src, int* hexielen);

#endif//__KEY_WORDS_TOOL_H__