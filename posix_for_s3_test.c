#include"posix_for_s3.h"
#include "utility.h"

int main()
{
    Meta_Data * meta_data=(Meta_Data*)malloc(sizeof(Meta_Data));
    meta_data->ioq.tail=0;
    meta_data->ioq.head=0;
    strcpy(meta_data->io_node_q_head,"");
    strcpy(meta_data->io_node_q_tail,"");
    //meta_data->replica_num=2;
    //strcpy(meta_data->my_rep[0].host_ip,"192.168.0.19");
    //strcpy(meta_data->my_rep[1].host_ip,"192.168.0.243");
    //strcpy(meta_data->my_rep[1].host_ip,"192.168.0.18");
    char * pathname="/mnt/supercache/hello.txt";
    struct stat stat_info;
    stat(pathname,&stat_info);
    meta_data->stat_info=stat_info;
    if(md_put(pathname,meta_data)!=0)
    {
        printf("Put meta_data in main error\n");
    }
    /*while(1)
    {
        if(md_get(pathname,meta_data)==0)
        {
        }
        else{printf("***************error************\n");}
    }*/
    pathname="/mnt/supercache/b.txt";
    if(md_put(pathname,meta_data)!=0)
    {
        printf("Put meta_data in main error\n");
    }
    pathname="/mnt/supercache/c.txt";
    if(md_put(pathname,meta_data)!=0)
    {
        printf("Put meta_data in main error\n");
    }
    
    return 0;
}
