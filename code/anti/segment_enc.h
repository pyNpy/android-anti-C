//
// Created by kl on 2019/1/29.
//

#ifndef NDKDEMO_SEGMENT_ENC_H
#define NDKDEMO_SEGMENT_ENC_H

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

void encode_sofile(const char* libname );
void decode_segment(const char *libname,const char* segment_name);
#ifdef __cplusplus
};
#endif
#endif //NDKDEMO_SEGMENT_ENC_H
