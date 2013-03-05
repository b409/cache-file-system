#include "io_queue.h"

u32 queue_in_wait(const char* filename,IO_Type io_type,time_t arrive_time)
{
    Meta_Data *meta_data=(Meta_Data*)malloc(sizeof(Meta_Data));
    if(md_get(filename,meta_data)==0)
    {
        u32 tail=(*meta_data).ioq.tail;
        (*meta_data).ioq.io_q_node[tail].io_type=io_type;
        (*meta_data).ioq.io_q_node[tail].arrive_time=arrive_time;
        (*meta_data).ioq.tail=((tail+1)%IO_Q_LEN);
        if(md_put(filename,meta_data)!=0)
        {
            printf("Put metadata error!\n");
            return 1;
        }
    }
    else
    {
        printf("Get metadata error!\n");
        return 1;
    }
  while(1)
    {
        if(md_get(filename,meta_data)==0)
        {
            IO_Q io_q=(*meta_data).ioq;
            u32 head=io_q.head;
            IO_Q_Node io_q_node=io_q.io_q_node[head];
            if((io_q_node.io_type==io_type)&&
                (io_q_node.arrive_time==arrive_time))
            {
                free(meta_data);
                return 0;
            }
        }
        else
        {
            printf("Get metadata error!\n");
            return 1;
        }
    }
}

u32 read_queuue_out(const char* filename,IO_Type io_type,time_t arrive_time)
{
    Meta_Data *meta_data=(Meta_Data*)malloc(sizeof(Meta_Data));
    if(md_get(filename,meta_data)==0)
    {
        u32 head=(*meta_data).ioq.head;
        (*meta_data).ioq.head=(head+1)%IO_Q_LEN;
        (*meta_data).stat_info.st_atime=arrive_time;
        if(md_put(filename,meta_data)!=0)
        {
            printf("Put metadata in read_queue_out error!\n");
            return 1;
        }
    }
    else
    {
        printf("Get metadata in read_queue_out error!\n");
        return 1;
    }
    return 0;
}
u32 write_queue_out(const char* filename,IO_Type io_type,time_t arrive_time,u64 offset,char *data,size_t size)
{
    char path[IO_NODE_KEY_LEN];
    sprintf(path, "%s_%ld", filename, arrive_time);  
    Meta_Data *meta_data=(Meta_Data*)malloc(sizeof(Meta_Data));
    if(md_get(filename,meta_data)==0)
    {
        /*modify ioq head and modification time in metadata*/
        u32 head=(*meta_data).ioq.head;
        (*meta_data).ioq.head=(head+1)%IO_Q_LEN;
        //(*meta_data).stat_info.st_mtime=arrive_time;
        //you should update the meta_data of the file in the real write action
        
        /*to add a new  io_node*/
        IO_Node *io_node=(IO_Node*)malloc(sizeof(IO_Node));
        (*io_node).io_type=io_type;
        (*io_node).offset=offset;
        (*io_node).io_time=arrive_time;
        strcpy((*io_node).ion_pre,(*meta_data).io_node_q_tail);
        //(*io_node).ion_next=NULL;
        if(ion_put(path,io_node)!=0)
        {
            printf("Put io_node in write_queue_out error!\n");
            return 1;
        }
        free(io_node);

        /*get out the tail io_node and change its next pointer*/
        if(strlen((*meta_data).io_node_q_tail)>1)
        {
            IO_Node *io_node1=(IO_Node*)malloc(sizeof(IO_Node));
            char* tail_path=(*meta_data).io_node_q_tail;
            if(ion_get(tail_path,io_node1)==0)
            {
                strcpy((*io_node1).ion_next,path);
                if(ion_put(tail_path,io_node1)!=0)
                {
                    printf("Put io_node in write_queue_out error!\n");
                    return 1;
                }
            }
            else
            {
                printf("Get io_node in write_queue_out error!\n");
                return 1;
            }
        }
        /*if the io_node_q is NULL*/
        if(strlen((*meta_data).io_node_q_head)==0)
        {
            strcpy((*meta_data).io_node_q_head,path);
            u32 i, replica_num=(*meta_data).replica_num;
            for(i=0;i<replica_num;i++)
            {
                strcpy((*meta_data).my_rep[i].io_node_ptr,path);
            }
        }
        strcpy((*meta_data).io_node_q_tail,path);
        if(md_put(filename,meta_data)!=0)
        {
            printf("Put metadata in write_queue_out error!\n");
            return 1;
        }
        free(meta_data);
    }
    else
    {
        printf("Get metadata in write_queue_out error!\n");
        return 1;
    }
    /* put the data in TT*/
    if(iod_put(path,data,size)!=0)
    {
        printf("Put data in write_queue_out error!\n");
        return 1;
    }
    /*send messages to the IPs*/
    return 0;
}

u32 remove_queuue_out(const char* filename,IO_Type io_type)
{

    /*send messages to the IPs*/
    /*delete the meta_data*/
    if(md_out(filename)!=0)
    {
        printf("Remove metadata in remove_queueout error!\n");
        return 1;
    }
    return 0;
}

u32 replica_update(const char *filename,const char*host_ip)
{
    /*here the host_ip should equals the one in the meta_data*/
    Meta_Data *meta_data=(Meta_Data*)malloc(sizeof(Meta_Data));
    if(md_get(filename,meta_data)==0)
    {
        /*to get the io_node_ptr of the right replica ip*/
        u32 i, replica_num=(*meta_data).replica_num;
        REPLICA replica;
        for(i=0;i<replica_num;i++)
        {
            replica=(*meta_data).my_rep[i];
            if(strcmp(replica.host_ip,host_ip)==0) break;
        }
        if(strlen(replica.io_node_ptr)!=0)
        {
            char io_node_ptr[IO_NODE_KEY_LEN];
            strcpy(io_node_ptr,replica.io_node_ptr);
            IO_Node *io_node=(IO_Node*)malloc(sizeof(IO_Node*));
            while(strlen(io_node_ptr)!=0)
            {
                if(ion_get(io_node_ptr,io_node)==0)
                {
                    u64 offset=(*io_node).offset;
                    u32 *data_size;
                    char * real_data=iod_get(io_node_ptr,data_size);

                    /*use the offset and data_size and real_data to update the replica*/

                    //free(real_data);
                    strcpy(io_node_ptr,(*io_node).ion_next);
                }   
                else
                {
                    printf("Get io_node in replica_update error!\n");
                    return 1;
                }
            }
            free(io_node);
            bzero((*meta_data).my_rep[i].io_node_ptr,IO_NODE_KEY_LEN);
        }
        /*do not forget to update the metadata*/
        if(md_put(filename,meta_data)!=0)
        {
            printf("Put metadata in replica_update error!\n");
            return 1;
        }
        free(meta_data);
    }
    else
    {
        printf("Get metadata in replica_update error!\n");
        return 1;
    }

}
