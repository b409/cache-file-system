#ifndef POSIX_FOR_S3
#define POSIX_FOR_S3
#include "glob.h"
#include "md_type.h"


int ReadOpen(const char* pathname,int flags);
FILE* WriteFopen(const char* pathname,const char* mode);
u32 CfRemove(const char* pathname);
#endif
