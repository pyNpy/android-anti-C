//
// Created by kl on 2019/1/29.
//

#ifndef NDKDEMO_SEGMENT_H
#define NDKDEMO_SEGMENT_H
#include <stdlib.h>
#include <string.h>
#include <sys/system_properties.h>
#include <elf.h>
#ifdef  __cplusplus
extern "C"{
#endif
unsigned long  getlibaddr(const char* libname);
bool is_ELF(unsigned long baseAddress );
int elf_class_size( unsigned long baseAddress);
#ifdef __cplusplus
};
#endif
#endif //NDKDEMO_SEGMENT_H
