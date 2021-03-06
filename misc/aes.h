///////////////////////////////////////////////////////////////////////////////
// file name:		aes.h
// author:			MinYou Game
// email:			MinYouGame@139game.net
// date created:	2011-06-29
// version:			1.0.0.0
// description:
///////////////////////////////////////////////////////////////////////////////
#pragma once


#ifndef AES_H_20110629
#define AES_H_20110629

#include <string.h>



#define AES_ENCRYPT						1
#define AES_DECRYPT						0

#define ERR_AES_OK						0
#define ERR_AES_INVALID_KEY_LENGTH		-0x0020		// Invalid key length
#define ERR_AES_INVALID_INPUT_LENGTH	-0x0022		// Invalid data input length


typedef struct
{
    int						nr;								// number of rounds
    unsigned long			*rk;							// AES round keys
    unsigned long			buf[68];						// unaligned data
} aes_context;


//---------------------------------------------------------------------------
// brief				AES key schedule (encryption)
// param ctx			AES context to be initialized
// param key			encryption key
// param keysize		must be 128, 192 or 256
// return				0 if successful, or ERR_AES_INVALID_KEY_LENGTH
int aes_setkey_enc(aes_context *ctx, const unsigned char *key, unsigned int keysize);

//---------------------------------------------------------------------------
// brief				AES key schedule (decryption)
// param ctx			AES context to be initialized
// param key			decryption key
// param keysize		must be 128, 192 or 256
// return				0 if successful, or ERR_AES_INVALID_KEY_LENGTH
int aes_setkey_dec(aes_context *ctx, const unsigned char *key, unsigned int keysize);

//---------------------------------------------------------------------------
// brief				AES-ECB block encryption/decryption
// param ctx			AES context
// param mode			AES_ENCRYPT or AES_DECRYPT
// param input			16-byte input block
// param output			16-byte output block
// return				0 if successful
int aes_crypt_ecb(aes_context *ctx, int mode, const unsigned char input[16], unsigned char output[16]);

//---------------------------------------------------------------------------
// brief				AES-CBC buffer encryption/decryption
//						Length should be a multiple of the block
//						size (16 bytes)
// param ctx			AES context
// param mode			AES_ENCRYPT or AES_DECRYPT
// param length			length of the input data
// param iv				initialization vector (updated after use)
// param input			buffer holding the input data
// param output			buffer holding the output data
// return				0 if successful, or ERR_AES_INVALID_INPUT_LENGTH
int aes_crypt_cbc(aes_context *ctx, int mode, size_t length, unsigned char iv[16], const unsigned char *input, unsigned char *output);

//---------------------------------------------------------------------------
// brief				AES-CFB128 buffer encryption/decryption.
// param ctx			AES context
// param mode			AES_ENCRYPT or AES_DECRYPT
// param length			length of the input data
// param iv_off			offset in IV (updated after use)
// param iv				initialization vector (updated after use)
// param input			buffer holding the input data
// param output			buffer holding the output data
// return				0 if successful
int aes_crypt_cfb128(aes_context *ctx, int mode, size_t length, size_t *iv_off, unsigned char iv[16], const unsigned char *input, unsigned char *output);

//---------------------------------------------------------------------------
// brief				AES-CTR buffer encryption/decryption
// Warning:				You have to keep the maximum use of your counter in mind!
// param length			The length of the data
// param nc_off			The offset in the current stream_block (for resuming
//                      within current cipher stream). The offset pointer to
//						should be 0 at the start of a stream.
// param nonce_counter	The 128-bit nonce and counter.
// param stream_block	The saved stream-block for resuming. Is overwritten
//                      by the function.
// param input			The input data stream
// param output			The output data stream
// return				0 if successful
int aes_crypt_ctr(aes_context *ctx, size_t length, size_t *nc_off, unsigned char nonce_counter[16], unsigned char stream_block[16], 
				  const unsigned char *input, unsigned char *output );
//---------------------------------------------------------------------------
//brief					Checkup routine
//return				0 if successful, or 1 if the test failed
int aes_self_test(int verbose);




#endif

