#ifndef _IO_QUEUE
#define _IO_QUEUE
#include "glob.h"
#include "md_type.h"
#include "tt_func.h"
u32 queue_in_wait(const char* filename,IO_Type io_type,time_t arrive_time);
u32 read_queue_out(const char* filename,IO_Type io_type,time_t arrive_time);
u32 write_queue_out(const char* filename,IO_Type io_type,time_t arrive_time,u64 offset,char *data,size_t size);
u32 replica_update(const char *filename,const char*host_ip);
#endif

