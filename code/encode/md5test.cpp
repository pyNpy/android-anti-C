//
// Created by kl on 2019/1/26.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "md5.h"
extern  void MD5Init(MD5_CTX *context);
extern void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen);
extern void MD5Final(MD5_CTX *context,unsigned char digest[16]);
extern void MD5Transform(unsigned int state[4],unsigned char block[64]);
extern void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len);
extern void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len);

int  testmd5() {
    MD5_CTX md5;
    MD5Init(&md5);
    int i;
    unsigned char encrypt[] ="admin";//21232f297a57a5a743894a0e4a801fc3
    unsigned char decrypt[16];
    MD5Update(&md5,encrypt,strlen((char *)encrypt));
    MD5Final(&md5,decrypt);
    printf("加密前:%s\n加密后:",encrypt);
    for(i=0;i<16;i++)
    {
        printf("%02x",decrypt[i]);
    }
    return 0;
}
