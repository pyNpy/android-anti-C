//
// Created by kl on 2019/1/26.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "anti/antiDebug.h"
extern  int ptraceself ();
int main()
{
    //ptraceself ();
    create_thread_check_TracerPid ();
    while(true){
        printf ("test\n");
        usleep(500000);
    }
    return 0;
}