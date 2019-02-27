//
// Created by kl on 2019/1/29.
//
#include "segment.h"
#include <stdio.h>
#include <fcntl.h>
#include <elf.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "../LOG.h"
#define BUFFLENGTH 4096
///proc/pid/maps libxx.so
extern  "C" unsigned long  getlibaddr(const char* libname) {
    char res[128]={'\0'};
    unsigned long baseAddress=0;
    sprintf (res,"/proc/%d/maps",getpid ());
    FILE* fp=fopen (res,"r");
    if(fp==NULL){
        LOGI("open %s file error",res);
        fclose (fp)  ;
        return  baseAddress;
    }
    char buff[BUFFLENGTH]={0};
    char * temp_point;

    while(fgets(buff, sizeof ( buff),fp)){
        if(strstr(buff,libname)!=NULL&& strstr (buff,"r-xp")!=NULL){
            temp_point=strtok (buff,"-");
            baseAddress =strtoul (temp_point,NULL,16);
            LOGI ("%s bassaddress: %x",libname ,baseAddress);
            break;
        }
    }
    fclose (fp);
    return baseAddress;

}
extern  "C" bool is_ELF(unsigned long baseAddress ) {

    unsigned char temp [4]={0};
    memcpy ((void*)baseAddress,temp,4);
//    unsigned char elf_[4]={0x7f,0x45 ,0x4c,0x46};
//    if(memcmp(elf_,temp,4)==0)
//        return true;
    if(memcpy ((void*)ELFMAG,temp,4)==0){
        return true;
    }
    return false;
}
//elf fize : 32bit  or 64bit
extern  "C" int elf_class_size( unsigned long baseAddress){
    unsigned char value='\0';
    memcpy ((void*)(baseAddress+5),(void*)value,1);

    switch (atoi ((char*)value)){
        case  ELFCLASS32:return ELFCLASS32 ;break;
        case  ELFCLASS64:return ELFCLASS64 ;break;
        case  ELFCLASSNONE:return ELFCLASSNONE ;break;
    }
    return ELFCLASSNONE;
}

