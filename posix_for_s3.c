#include "posix_for_s3.h"
ssize_t CfRead(int fd,void* buf,size_t count,const char* pathname)
{
    time_t arrive_time;
    IO_Type io_type=READ;
    queue_in_wait(pathname,io_type,arrive_time);
    u32 already_queue_out=0;//the mark of read out early

     Meta_Data * meta_data=(Meta_Data*)malloc(sizeof(Meta_Data));
     if(md_get(pathname,meta_data)==0)
     {
         u32 head_next=((*meta_data).ioq.head+1)%IO_Q_LEN;
         /*if one read comes after one read ,queue out early*/
         if((*meta_data).ioq.io_q_node[head_next].io_type==READ)
         {
             read_queue_out(pathname,io_type,arrive_time);
             already_queue_out=1;
         }
     }
     int ret=1;
     if(already_queue_out==0)
     {
         read_queue_out(pathname,io_type,arrive_time);
     }
     return ret;
}

size_t CfFwrite(const void* ptr,size_t size,size_t nmemb,FILE *stream,const char* pathname)
{
    time_t arrive_time;
    IO_Type io_type=WRITE;
    queue_in_wait(pathname,io_type,arrive_time);
    
    size_t ret=fwrite(ptr,size,nmemb,stream);//do the real write.
    
    u64 offset=0;char *data="never mind!";size_t size1=11;
    write_queue_out(pathname,io_type,arrive_time,offset,data,size1);//should change it
    return ret;
}

u32 CfRemove(const char* pathname)
{
    time_t arrive_time;
    IO_Type io_type=REMOVE;
    queue_in_wait(pathname,io_type,arrive_time);
    u32 ret=remove(pathname);//do real remove action
    remove_queue_out(pathname,io_type,arrive_time);
    return ret;
}
