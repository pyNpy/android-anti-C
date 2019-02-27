//
// Created by kl on 2019/1/23.
//
#include<signal.h>
#include<sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ptrace.h>
#include "antiDebug.h"

#include "../LOG.h"
#define BUF_LEN 1024
/**
 * 获取 /proc/pid/cmdline  启动app名
 * */
const char*  getAppCmdline(int pid)
{
    char res[BUF_LEN]={0};
    char cmd[BUF_LEN]={0};
    sprintf(cmd,"/proc/%d/cmdline",pid);
    int  fd = open(cmd,O_RDONLY);
    if(read(fd,res,BUF_LEN)) {
        LOGI("%s:%s",cmd,res);
    }else{
        LOGI("error:read%s",cmd);
    }
    close(fd);
    return res;
}
/**
 * main app has the dir /data/data/com.xx.xx
 * */
int is_main_app(int pid ) {
    const char * res = getAppCmdline( pid );
    char targetPath[BUF_LEN]={0};
    sprintf(targetPath,"/data/data/%s",res);
    if(access(targetPath,F_OK)==0){
        return 1;
    }
    return 0;
}

//int fork_C_for() {
//    if(app_notfork()){
//        return -1;
//    }
//    //...
//    //__fork(...);
//}
/*ptrace self */
int ptraceself() {
    ptrace(PTRACE_TRACEME,getpid(),NULL,NULL);
}

/**
 * 根据/proc/pid/status ，TracerPid判断是否在被调试
 * **/
//获取TracePid
int getNumfromString(char* str){
    if(str == NULL){
        return -1;
    }
    char result[20];
    int count = 0;
    while(*str != '\0'){
        if(*str >= 48 && *str <= 57){
            result[count] = *str;
            count++;
        }
        str++;
    }
    int val = atoi(result);
    return val;
}
int getTracerPid(int pid )
{
    char statusfile[BUF_LEN]={0};
    sprintf(statusfile,"/proc/%d/status",pid);
    FILE* fp=fopen (statusfile,"r");
    char linestr[BUF_LEN]={'\0'};
    int TracerPid=0;
    if(fp!=NULL){
        int i=0;
        while(!feof (fp)){
            fgets (linestr,BUF_LEN,fp);
            if(i==5){
                TracerPid=getNumfromString(linestr);
                break;
            }
            i++;
        }
    }
    fclose (fp);
    return TracerPid;
}

int killtarget(int pid)
{
    kill (pid,SIGKILL); //kill(pid,SIGKILL);
}
void killProcess_c()
{
    int pid =getpid ();
    kill (pid,SIGKILL);
    int status=0;
    wait (&status);
    if(WIFSIGNALED (status)){
        LOGI("kill pid:%d",pid);
        printf ("kill pid:%d",pid);
    }
}

void * thread_function(void*arg)
{
    while( true){
        int TracerPid=0;
        TracerPid=getTracerPid (getpid ());
        printf ("pid=%d TracerPid=%d \n",getpid (),TracerPid);
        if(TracerPid>0){
//            printf ("pid=%d TracerPid=%d",getpid (),TracerPid);
            sleep (2);
            kill (getpid (),SIGKILL);
            //exit(0);
        }
        sleep (3);
    }
}

pthread_t thread_id;
void create_thread_check_TracerPid()
{
    int err=pthread_create(&thread_id,NULL, thread_function, NULL);
    printf ("create check tracer pid  thread\n");
    LOGI("create check tracer pid  thread\n");
    if(err!=0){
        printf ("create check tracer pid  thread fail \n");
    }
}

void killProcess_java(JNIEnv*env, jobject jcontext)
{
    jclass  cProcess = env->FindClass ("android/os/Process");
    jmethodID myPid_id=env->GetStaticMethodID (cProcess,"myPid","()I");
    jint pid= env->CallStaticIntMethod (cProcess,myPid_id);
    jmethodID  killProcess_id=env->GetStaticMethodID (cProcess,"killProcess","(I)V");
    LOGI("kill process ");
    env->CallStaticVoidMethod (cProcess,killProcess_id,pid);
}
jboolean isdebugging_java(JNIEnv*env, jobject jcontext)
{
//    android.os.Debug.isDebuggerConnected();
    jclass  cDebug = env->FindClass ("android/os/Debug");
    jmethodID isDebuggerConnected_id=env->GetStaticMethodID (cDebug,"isDebuggerConnected","()Z");
    jboolean  isDebuggerConnected =env->CallStaticBooleanMethod (cDebug,isDebuggerConnected_id);
    return isDebuggerConnected;
}