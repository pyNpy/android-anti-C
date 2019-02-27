//
// Created by kl on 2019/1/23.
//

#ifndef NDKDEMO_ANTIDEBUG_H
#define NDKDEMO_ANTIDEBUG_H

#include <string>
#include <stdlib.h>
#include <jni.h>
using  namespace std;
const char*getAppCmdline(int pid);
int is_main_app(int pid );
int ptraceself();
int getTracerPid(int pid );
int killtarget(int pid);
void killappSelf();
void thread_function();
void create_thread_check_TracerPid();

void killProcess_java(JNIEnv*env, jobject jcontext);
jboolean isdebugging_java(JNIEnv*env, jobject jcontext);


#endif //NDKDEMO_ANTIDEBUG_H
