// test_curl_rsa.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "crul/curl.h"
#pragma comment(lib, "libcurl.lib")

char* url = "http://42.62.65.44/sms/rsa_en.aspx?notify_data=just%20a%20testjust%20a%20testjust!!123";

struct MemoryStruct
{
    MemoryStruct()
    {
        memory = 0;
        size = 0;
    }
    char *memory;
    size_t size;
};


static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        exit(EXIT_FAILURE);
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void http_get(char* uri, MemoryStruct* data)
{
    curl_global_init(CURL_GLOBAL_ALL);

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, uri);

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, FALSE);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
        curl = 0;
    }

    /* we're done with libcurl, so clean it up */
    curl_global_cleanup();

}


int _tmain(int argc, _TCHAR* argv[])
{
    MemoryStruct data;
    http_get(url, &data);


	return 0;
}

