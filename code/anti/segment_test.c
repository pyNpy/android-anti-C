//
// Created by kl on 2019/1/26.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <unistd.h>
#include "segment_enc.h"

int main(int argc,const char* args[])
{
    int i;
    for (  i = 0 ; i < argc ; ++i ) {
        printf("%s \n",args[i]);
    }
    encode_sofile ((const char*)args[1]);
    return 0;

}





