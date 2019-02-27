//
// Created by kl on 2019/1/26.
//

#ifndef NDKDEMO_CHECKSIGNATURE_H
#define NDKDEMO_CHECKSIGNATURE_H
#include <jni.h>

int  checksignature(JNIEnv*env, jobject jcontext  );
char* getsignature(JNIEnv*env, jobject jcontext );



#endif //NDKDEMO_CHECKSIGNATURE_H
