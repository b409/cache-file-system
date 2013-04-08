#include "posix_func.h"
#include "glob.h"
#include "md_type.h"
#include "utility.h"
#include "io_queue.h"


u32 _RemoveFile(u8 *path)
{
    char cache_path[FILE_PATH_LEN];
    /*****************************************************
     * you should do some hash to the path
     * **************************************************/
    get_cache_path(path,cache_path);
    u32 ret;
    time_t arrive_time;
    IO_Type io_type=REMOVE;
    queue_in_wait(path,io_type,arrive_time);

    if(unlink(cache_path)!=0)
    {
        printf("Remove file in _RemoveFile error!\n");
        return 1;
    }

    remove_queue_out(path,io_type,arrive_time);
    return 0;
    
}

u32 _ReadFile(u8 *path, u8 *buf, u32 count, u64 offset)
{
    
    time_t arrive_time;
    IO_Type io_type=READ;
    queue_in_wait(path,io_type,arrive_time);
    u32 already_queue_out=0;//the mark of read out early

     Meta_Data * meta_data=(Meta_Data*)malloc(sizeof(Meta_Data));
     if(md_get(path,meta_data)==0)
     {
         u32 head_next=((*meta_data).ioq.head+1)%IO_Q_LEN;
         /*if one read comes after one read ,queue out early*/
         if((*meta_data).ioq.io_q_node[head_next].io_type==READ)
         {
             read_queue_out(path,io_type,arrive_time);
             already_queue_out=1;
         }
     }
    
    u8 cache_path[FILE_PATH_LEN];
    /*****************************************************
     * you should do some hash to the path
     * **************************************************/
    get_cache_path(path,cache_path);
    u32 fd,read_size;
    if((fd=open(cache_path,O_RDONLY))==-1)
    {
        printf("Open file in _ReadFile error!\n");
        return 1;
    }
    if((read_size=read(fd,buffer,count))==-1)
    {
        printf("Read file in _ReadFile error!\n");
        return 1;
    }

     if(already_queue_out==0)
     {
         read_queue_out(path,io_type,arrive_time);
     }
}

u32 _WriteFile(u8 *path, u8 * buf, u32 count, u64 offset)
{
    
}
