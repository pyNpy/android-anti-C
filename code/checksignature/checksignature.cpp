//
// Created by kl on 2019/1/26.
//
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include "../LOG.h"
#include <jni.h>
#include "checksignature.h"
#include "../encode/md5.h"
const char* appsignture="14ccaf8bec71b6631180af81b3b73bfc";//md5(signatures[0].toCharsString)
int  checksignature(JNIEnv*env, jobject jcontext  )
{
    unsigned  char md5string[128]={0};
    unsigned  char * sign = ( unsigned char*)getsignature (env,jcontext) ;
    encodemd5(sign,md5string);
    if(strcmp ((const char*)md5string ,appsignture)==0){
        return 1;
    }
    return 0;
}

////得到签名
//PackageInfo packageInfo = getPackageManager()
//        .getPackageInfo
//                (getPackageName(),
//                 PackageManager.GET_SIGNATURES);
//Signature[] signs = packageInfo.signatures;
//for (int i=0;i<signs.length;i++)
//Log.i("sign",new String(signs[i].toCharsString()));

char* getsignature(JNIEnv*env, jobject jcontext )
{
    jclass classContext = env->GetObjectClass (jcontext);
    jmethodID  getPackageManager_id =env->GetMethodID (classContext,"getPackageManager","()Landroid/content/pm/PackageManager;");
    jobject mPackageManager =env->CallObjectMethod (jcontext,getPackageManager_id);
    jclass  PackageManager_class =env->GetObjectClass (mPackageManager);
    jobject  mPackageName= env->CallObjectMethod ( jcontext,env->GetMethodID (classContext,"getPackageName","()Ljava/lang/String;") );
    jmethodID getPackageInfo_id=env->GetMethodID (PackageManager_class,
            "getPackageInfo","(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    //PackageManager.GET_SIGNATURES源码就可以知道了0x00000040==PackageManager.GET_SIGNATURES
    jobject mPackageinfo=env->CallObjectMethod (mPackageManager,getPackageInfo_id,mPackageName,0x00000040);

    jfieldID Signature_id=env->GetFieldID (env->GetObjectClass (mPackageinfo),"signatures","[Landroid/content/pm/Signature;");
    jobjectArray  mSignatureArray=(jobjectArray)env->GetObjectField (mPackageinfo,Signature_id);
    jobject  mSignature=env->GetObjectArrayElement (mSignatureArray,0);
    jstring  signature_string=(jstring)env->CallObjectMethod (
            mSignature,
            env->GetMethodID (env->GetObjectClass (mSignature),"toCharsString","()Ljava/lang/String;")
           );
    char* c_signature_str  = (char*)env->GetStringUTFChars (signature_string,NULL);
    return c_signature_str ;
}

