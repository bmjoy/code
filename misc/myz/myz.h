#ifndef __MY_Z_H__
#define __MY_Z_H__


class i_encrypt_7z
{
public:
    virtual int __stdcall encrypt_7z(const char* pakname, const char* filename, char* data, int len, const char* pwd) = 0;
};

extern "C"{
    int __stdcall encrypt_7z(const char* pakname, const char* filename, char* data, int len, const char* pwd);
    i_encrypt_7z* __stdcall create7z();
    void __stdcall release7z(i_encrypt_7z* p);
}

void test_myz();

#endif//__MY_Z_H__