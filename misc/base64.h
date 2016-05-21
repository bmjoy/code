///////////////////////////////////////////////////////////////////////////////
// file name:		base64.h
// author:			MinYou Game
// email:			MinYouGame@139game.net
// date created:	2011-06-29
// version:			1.0.0.0
// description:
///////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef BASE64_H_20110629
#define BASE64_H_20110629

enum base64_result
{
	CRYPT_OK,
	CRYPT_BUFFER_OVERFLOW = 6,
	CRYPT_INVALID_PACKET
};

int base64_encode(const unsigned char *in,  int len, unsigned char *out, int *outlen);
int base64_decode(const unsigned char *in,  int len, unsigned char *out, int *outlen);


#endif
