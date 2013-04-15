#ifndef POSIX_FOR_S3
#define POSIX_FOR_S3
#include "glob.h"
#include "md_type.h"

ssize_t CfRead(int fd,void* buf,size_t count,const char* pathname);

size_t CfFwrite(const void* ptr,size_t size,size_t nmemb,FILE *stream,const char* pathname);

u32 CfRemove(const char* pathname);
#endif
