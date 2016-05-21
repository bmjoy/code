#include "kw.h"
#include "stack_sort.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <list>
#include <string>
#include <vector>
#include <set>
#include "character_interface.h"
#include<algorithm>

using namespace std;
#pragma warning(disable:4996)

#define MAX_KW_LEN 64
#define MAX_KW_BUF_LEN 128
#define BUF_LEN 1024

/*
#ifdef _WIN32
#include <atlstr.h>
#define MS2WS(out, outlen, src, srclen) outlen = ::MultiByteToWideChar(CP_ACP, NULL, src, srclen, NULL, 0);	::MultiByteToWideChar(CP_ACP, NULL, src, srclen, out, outlen);out[outlen] = 0;
#define WS2MS(out, outlen, src, srclen) ::WideCharToMultiByte(CP_OEMCP,NULL,src, -1, out, MAX_KW_LEN-1, NULL,FALSE); outlen = strlen(out);
#else
#define MS2WS(out, outlen, src, srclen) outlen = mbstowcs(out, src, srclen); out[outlen] = 0;
#define WS2MS(out, outlen, src, srclen) outlen = wcstombs(out, src, srclen); out[outlen] = 0;
#endif
#ifdef _WIN32
#pragma warning(disable:4996)
#endif
*/
struct key_word_item
{
    char word[MAX_KW_LEN];
};

struct key_wcword_item
{
    wchar_t word[MAX_KW_LEN];
	bool operator<(const key_wcword_item&other) const
	{
		return wcscmp(word,other.word)<0;
	}
};

void make_lower(wchar_t* s, int len)
{
	static const wchar_t tag[3] = {65,65313,65345};
	for(int i=0; i<len; i++)
	{
		for(int j=0; j<3; j++)
		{
			if(s[i]>=tag[j] && s[i]<=(tag[j]+25))
			{
				s[i] = 97 + (s[i] - tag[j]);
				break;
			}
		}
	}
}

int remove_space(char* s, int len)
{
    int index = 0;
    for(int i=0; i<len; i++)
    {
        if(s[i] != ' ')
        {
            s[index] = s[i];
            index ++;
        }
    }
    s[index] = 0;
    return index;
}

int sfind(const char* s1, int len1, const char* s2, int len2)
{
    if(len1 > len2)
        return -1;
    for(int i=0; i<=len2-len1; i++)
    {
        bool b = true;
        for(int j=0; j<len1; j++)
        {
            if(s1[j] != s2[i+j])
            {
                b = false;
                break;
            }
        }
        if(b)
            return i;
    }
    return -1;
}

void sort_key_word(key_word_item* words, int size)
{
    sort_buff_helper<int, key_word_item*> helper;
    helper.create(size);
    key_word_item* temp = new key_word_item[size];
    memcpy(temp, words, size * sizeof(key_word_item));
    for(int i=0; i<size; i++)
    {
        helper[i].value = strlen(temp[i].word);
        helper[i].d = &temp[i];
    }
    stack_sort(helper, size);
    for(int i=0; i<size; i++)
    {
        memcpy(words[i].word, helper[i].d->word, sizeof(key_word_item));
    }
    delete temp;
}
int kw_optimize(const char* srcfilename, const char* outfile)
{
    FILE* fp_in = fopen(srcfilename, "r");

    if(!fp_in)
        return -1;
    char inword[MAX_KW_LEN];
    printf("loading the key words from: %s\n", srcfilename);
    int lines = 0;
    while(fgets(inword, MAX_KW_LEN, fp_in)) lines ++;
    fseek(fp_in, SEEK_SET, 0);
    key_wcword_item* words = new key_wcword_item[lines];
    int index = 0;
	char tempword[MAX_KW_LEN];
    while(fgets(tempword, MAX_KW_LEN, fp_in))
    {
        int len = strlen(tempword);
        while(len > 1 && ((tempword[len - 1] == '\r') || (tempword[len - 1] == '\n')))
        {
            len --;
        }
        tempword[len] = 0;
		int wscLen = _gbk_to_unicode(tempword,len,words[index].word,MAX_KW_LEN);
		make_lower(words[index].word, wscLen);
		
		
        index ++;
    }
    fclose(fp_in);
    printf("sorting the word by the length.\n");
    //sort_key_word(words, index);
	sort(words,words+index);
    
    list<key_wcword_item*> srcWords;
    list<key_wcword_item*> dstWords;
    for(int i=0; i<index; i++) srcWords.push_back(&words[i]);
    printf("erase duplicated words\n");
    while(srcWords.size() > 0)
    {
        key_wcword_item* t = *srcWords.begin();
        srcWords.pop_front();
        for(list<key_wcword_item*>::iterator i=srcWords.begin(); i!=srcWords.end(); )
        {
			wchar_t * pwc=wcsstr(t->word,(*i)->word);
			if(NULL!=pwc)
				i = srcWords.erase(i);
			else
				i++;

        }
        dstWords.push_back(t);
    }

    FILE* fp_out = fopen(outfile, "w");
    printf("output result to : %s\n", outfile);
	for(list<key_wcword_item*>::iterator ii=dstWords.begin(); ii!=dstWords.end(); ii++)
	{
		_unicode_to_gbk((*ii)->word,wcslen((*ii)->word),tempword,MAX_KW_LEN);
		fwrite(tempword, 1, strlen(tempword), fp_out);
		fwrite("\r\n", 1, 2, fp_out);
	}
	fclose(fp_out);
	delete words;
    printf("optimize key words library file ok.\n");
    return 0;
}

struct IndexItem
{
	int start;
	int end;
};

IndexItem g_index[65536];

struct KeyWordsItem
{
	wchar_t words[MAX_KW_LEN];
	char    cwords[MAX_KW_LEN];
	int     len;
	bool operator<(const KeyWordsItem&other) const
	{
		return wcscmp(words,other.words)<0;
	}

};

vector<KeyWordsItem> g_KeyWords;
set<KeyWordsItem> s_KeyWords;

void add_key_word(const char* s, int len)
{
	wchar_t wszString[MAX_KW_BUF_LEN*2] = {0};
	char szWordBuf[MAX_KW_LEN];
	if(len > 32)
		return;
	memcpy(szWordBuf, s, len);
	szWordBuf[len] = 0;
    len = remove_space(szWordBuf, len);
    
    int wscLen;
    //MS2WS(wszString, wscLen, szWordBuf, strlen(szWordBuf));
    wscLen = _gbk_to_unicode(szWordBuf, strlen(szWordBuf), wszString, MAX_KW_BUF_LEN*2);
	make_lower(wszString, wscLen);
	
	KeyWordsItem item;
    memset(&item, 0, sizeof(item));
	wcscpy(item.words, wszString);
	strcpy(item.cwords, s);
	item.len = wcslen(item.words);

	s_KeyWords.insert(item);

}

void sort_key_words_index()
{
	for(set<KeyWordsItem>::iterator it=s_KeyWords.begin();it!=s_KeyWords.end();it++)
	{
		g_KeyWords.push_back(*it);
	}
	int n = g_KeyWords.size();
    //printf("sorting \n");
    
    sort_buff_helper<int, KeyWordsItem*> helper;
    KeyWordsItem* sortItem = new KeyWordsItem[n];
    helper.create(n);
    for(int i=0; i<n; i++)
    {
        sortItem[i] = g_KeyWords[i];
        helper[i].value = sortItem[i].words[0];
        helper[i].d = &sortItem[i];
    }
    /*stack_sort(helper, n);
    for(int i=0; i<n; i++)
        g_KeyWords[i] = *helper[i].d;*/

    //printf("creating index matrix.\n");
	for(int i=0; i<65536; i++)
	{
		g_index[i].start = -1;
		g_index[i].end = -1;
	}
	unsigned short c = 0xffff;
	unsigned short t = 0xffff;
    t = g_KeyWords[0].words[0];
    c = t;
    g_index[c].start = 0;
    int vn = 0;
	for(int i=1; i<n; i++)
	{
		t = g_KeyWords[i].words[0];
		if(c != t)
		{
		    vn ++;
			g_index[c].end = i;
            g_index[t].start = i;
			c = t;
		}
	}
	printf("vn = %d\n", vn);
}

int kw_init(const char* kw_file)
{
    FILE* file = fopen(kw_file, "r");
    if(!file)
        return -1;
    char buf[MAX_KW_LEN];
	g_KeyWords.clear();
    while(fgets(buf, MAX_KW_LEN, file))
    {
        int len = strlen(buf);
        while(len > 1 && (buf[len-1]==13 || buf[len-1] == 10))
        {
            len --;
        }
        buf[len] = 0;
        add_key_word(buf, len);
    }
    sort_key_words_index();
    return 0;
}

int kw_check(const char* word, char* hint, int* hintlen)
{
	wchar_t wszString[2048];
	char szWordBuf[2048] = {0};

	int msgBufLen = strlen(word);
	if(msgBufLen>=1024)
	{
		sprintf(hint, "²ÎÊý³¬³¤");
		*hintlen = (int)strlen(hint);
		return -1;
	}

	strcpy(szWordBuf, word);
	
	int len = remove_space(szWordBuf, strlen(szWordBuf));

    int wcsLen;
    //MS2WS(wszString, wcsLen, szWordBuf, len);
    wcsLen = _utf8_to_unicode(szWordBuf, len, wszString, 2048);
	make_lower(wszString, wcsLen);
	//bool bHeXie = false;
	int index = 0;
	//int oIndex= 0;

	int keyLen = g_KeyWords.size();

	for(;index<wcsLen;)
	{
		wchar_t c = wszString[index];
		bool bFind = false;
		unsigned short cindex = c;
		int start;
		int end;
		start = g_index[cindex].start;
		end = g_index[cindex].end;
		if(end<0)
			end = keyLen;
		if(start>=0)
		{
			for(int j=start; j<end; j++)
			{
				if((wcsLen-index)>=g_KeyWords[j].len)
				{
					bFind = true;
					for(int k=0; k<g_KeyWords[j].len; k++)
					{
						if(wszString[index+k] != g_KeyWords[j].words[k])
						{
							bFind = false;
							break;
						}
					}
					if(bFind)
					{
						sprintf(hint, "%s", g_KeyWords[j].cwords);
						return -1;
					}
				}
			}
		}
		index ++;
	}
	return 0;
}

int kw_hexie(char* msg, int* hexielen)
{
    int len = strlen(msg);
	if(len>1024)
		return 0;
    char    mbuf[2048];
	wchar_t wszString[2048] = {0};
	wchar_t temp[2048] = {0};
    memcpy(mbuf, msg, len);
    
    
    len = remove_space(mbuf, len);
	int wcsLen;
	//MS2WS(wszString, wcsLen, mbuf, strlen(mbuf));
    wcsLen = _utf8_to_unicode(mbuf, len, wszString, 2048);
	make_lower(wszString, wcsLen);
	int keySize = g_KeyWords.size();
	int hexieLen = 0;
	bool bHeXie;
	bool bb = false;
	int index = 0;
	int tIndex= 0;
	for(; index<wcsLen; )
	{
		wchar_t c = wszString[index];
		bHeXie = false;

		unsigned short cindex = c;
		int start;
		int end;
		start = g_index[cindex].start;
		end = g_index[cindex].end;
		if(end<0)
			end = keySize;
		bool bFind;
		if(start>=0)
		{
			for(int j=start; j<end; j++)
			{
				if((c == g_KeyWords[j].words[0]) && (wcsLen-index)>=g_KeyWords[j].len)
				{
					bFind = true;
					for(int k=0; k<g_KeyWords[j].len; k++)
					{
						if(wszString[index+k] != g_KeyWords[j].words[k])
						{
							bFind = false;
							break;
						}
					}
					if(bFind)
					{
						temp[tIndex] = '*';
						temp[tIndex+1] = '*';
						hexieLen = g_KeyWords[j].len;
						bHeXie = true;
						break;
					}
				}
			}
		}

		if(bHeXie)
		{
			bb = true;
			index += hexieLen;
			tIndex += 2;
		}
		else
		{
			temp[tIndex] = c;
			index ++;
			tIndex ++;
		}
	}

	if(bb)
	{
	    //WS2MS(msg, *hexielen, temp, tIndex);
	    *hexielen = _unicode_to_utf8(temp, tIndex, msg, len*2);
	    return -1;
	}
	return 0;
}
