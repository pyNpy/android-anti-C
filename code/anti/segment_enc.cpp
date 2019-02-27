//
// Created by kl on 2019/1/29.
//
#include "segment_enc.h"
#include <stdio.h>
#include <fcntl.h>
#include <elf.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "../LOG.h"
#include "segment.h"

extern "C"  void encode_sofile(const char* libname ) {
    printf("open %s \n",libname);
    const char * target_section="xxx";
    char *shstr = NULL;
    char *content = NULL;
    Elf32_Ehdr ehdr;
    Elf32_Shdr shdr;
    int i;
    unsigned int base, length;
    unsigned short nblock;
    unsigned short nsize;
    unsigned char block_size = 16;

    int fd;
    fd = open(libname, O_RDWR);
    if(fd < 0){
        printf("open %s failed\n", libname );
        goto _error;
    }

    if(read(fd, &ehdr, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr)){
        puts("Read ELF header error");
        goto _error;
    }

    //str segment
    lseek(fd, ehdr.e_shoff + sizeof(Elf32_Shdr) * ehdr.e_shstrndx, SEEK_SET);
    if(read(fd, &shdr, sizeof(Elf32_Shdr)) != sizeof(Elf32_Shdr)){
        puts("Read ELF section string table error");
        goto _error;
    }
    if((shstr = (char *) malloc(shdr.sh_size)) == NULL){
        puts("Malloc space for section string table failed");
        goto _error;
    }
    lseek(fd, shdr.sh_offset, SEEK_SET);
    if(read(fd, shstr, shdr.sh_size) != shdr.sh_size){
        puts("Read string table failed");
        goto _error;
    }
    //

    lseek(fd, ehdr.e_shoff, SEEK_SET);
    for(i = 0; i < ehdr.e_shnum; i++){
        if(read(fd, &shdr, sizeof(Elf32_Shdr)) != sizeof(Elf32_Shdr)){
            puts("Find section .text procedure failed");
            goto _error;
        }
        if(strcmp(shstr + shdr.sh_name, target_section) == 0){
            base = shdr.sh_offset;
            length = shdr.sh_size;
            printf("Find section %s\n", target_section);
            break;
        }
    }
    lseek(fd, base, SEEK_SET);
    content = (char*) malloc(length);
    if(content == NULL){
        puts("Malloc space for content failed");
        goto _error;
    }
    if(read(fd, content, length) != length){
        puts("Read section .text failed");
        goto _error;
    }
    /* *
     *   entry is for decode the segment ,for base length
     *
     *
     * */
    nblock = length / block_size;
    nsize = base / 4096 + (base % 4096 == 0 ? 0 : 1);
    printf("base = %4x, length = %d\n", base, length);
    printf("nblock = %d, nsize = %d\n", nblock, nsize);

    ehdr.e_entry = (length << 16) + nsize;
    ehdr.e_shoff = base;

    //encode the segment "xxx" code
    for(i=0;i<length;i++){
        content[i] = ~content[i];
    }

    lseek(fd, 0, SEEK_SET);
    if(write(fd, &ehdr, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr)){
        puts("Write ELFhead to .so failed");
        goto _error;
    }

    lseek(fd, base, SEEK_SET);
    if(write(fd, content, length) != length){
        puts("Write modified content to .so failed");
        goto _error;
    }
    puts("Completed");

  _error:
    free(content);
    free(shstr);
    close(fd);

}
void encode_function(){


}




extern  "C" void decode_segment(const char *libname,const char* segment_name){
    unsigned long  libAddr =getlibaddr (libname);
    unsigned int nblock= 0;
    unsigned int nsize=0;
    Elf32_Ehdr*  elf_hdr=NULL;
    Elf32_Shdr* segment_shder=NULL;
    Elf32_Phdr* program_phdr=NULL;
    unsigned  long textcode_addr;
    elf_hdr=(Elf32_Ehdr*)libAddr;
    textcode_addr=libAddr+elf_hdr->e_shoff;
    nblock = elf_hdr->e_entry>>16;
    nsize=elf_hdr->e_entry&0xFFFF;
    LOGI("nclock = %d \n",nblock);
    LOGI("nsize = %d \n",nsize);
    if(mprotect ((void*)libAddr,4096*nsize,PROT_WRITE|PROT_READ|PROT_EXEC)!=0){
        LOGI("mem privilege change failed");
    }
    int i;
    for(i=0;i< nblock; i++){
        char *addr = (char*)(textcode_addr + i);
        *addr = ~(*addr);
    }
    if(mprotect ((void*)libAddr,4096*nsize,PROT_READ|PROT_EXEC)!=0){
        LOGI("mem privilege change failed");
    }
}





