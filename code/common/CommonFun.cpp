//
// Created by kl on 2019/1/26.
//
#include "CommonFun.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
/**
 * src: whole string
 * separator: eg:":"
 * dest:
 * **/
int splitStrings(char *src,const char *separator,char **dest) {
    char *pNext;
    int count = 0;
    if (src == NULL || strlen(src) == 0)
        return count;
    if (separator == NULL || strlen(separator) == 0)
        return count;
    pNext = strtok(src,separator);
    while(pNext != NULL) {
        *dest++ = pNext;
        ++count;
        pNext = strtok(NULL,separator);
    }
    return count;
}


