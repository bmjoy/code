#include "stdlib.h"
#include "curl/curl.h"
#include "http_get_string.h"
#pragma comment(lib, "libcurl.lib")


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

void http_get(const char* uri, MemoryStruct* data)
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

int get_http_string(const char* url, char* buf, int buflen)
{
    MemoryStruct data;
    http_get(url, &data);

    if(data.size<=0 || !data.memory)
        return 0;
    if(data.size>(size_t)(buflen-1))
        return -1;
    int size = data.size;
    memcpy(buf, data.memory, size);
    buf[size] = 0;
    free(data.memory);
    return size;
}