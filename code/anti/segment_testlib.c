//
// Created by kl on 2019/1/26.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <unistd.h>
#include "segment_enc.h"
#include "../LOG.h"
/**
 * constructor (priority)
 * 可以在后面指定一个优先级，
 * 数字越小，优先级越高，越先被执行。
 * */
void init_1() __attribute__((constructor(1)));
void init_2() __attribute__((constructor(2)));
void init_3() __attribute__((constructor(3)));

//__attribute__((section (".init_array"))) const char* init_value="t000000_init_array";
__attribute__((section ("xxx2"))) const char* xxx2value="segment_test";
__attribute__((section ("xxx"))) void  segment_test(){
    LOGI ("segment xxx: segment_test");
    printf("%s",xxx2value);
    printf ("xxxx2312313xx");
    printf ("%d",213);
}
#define CharSize 128
char  buff[CharSize] ={'\0'} ;
void init_1(){
    LOGI("init_1");
    buff[0]='h';
    buff[1]='h';
    buff[2]='\0';
    printf ("%s",buff);

}
void init_2() {
    LOGI("init_2");
    printf ("%s",buff);
    decode_segment ("libnative-lib.so","xxx");
}
void init_3() {
    LOGI("init_3");
    sprintf (buff,"%s","the init_3 function");
    printf ("%s",buff);
    segment_test();
}

