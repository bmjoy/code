#ifndef __HID_CRYPT_H__
#define __HID_CRYPT_H__

#ifdef __cplusplus
extern "C" {
#endif

extern void hid_encode(char* src, int slen, char* dst, int* dlen);
extern int  hid_decode(char* src, int slen, char* dst, int* dlen);
extern void test_hid_crypt();

#ifdef __cplusplus
}
#endif

#endif//__HID_CRYPT_H__