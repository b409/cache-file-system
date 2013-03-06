/*
 * File: io_queue.c
 * BY: Jin
 * Time: 2013-03-06
 *
 * */
#include "io_queue.h"
#include "sn_sckt.h"
#include "xml_msg.h"

/* 
*  Used to put the IO into the queue and make the process waiting the IO to be the head of the queue.
*  Parameters: filename--the key to get the metadata.
*              io_type--READ? WRITE? REMOVE?
*              arrive_time--the curruent time.
*  Returns: 1--wrong
*           0--This IO become the head of the queue. Now you can do the Exac IO
*/
u32 queue_in_wait(const char* filename,IO_Type io_type,time_t arrive_time)
{
    Meta_Data *meta_data=(Meta_Data*)malloc(sizeof(Meta_Data));

    //To get the metadata,then change it and update it.
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

 //to wait the IO to be the head.
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

/*
 * Used to do the read queue out.It is different to the write queue out.So detach it
 * Parameters: arrrive_time--use it to change the stat_info (st_atime)
 * Returns: 1--wrong  0--ok
 * Attention:
 *         (1) if it's RR model ,call the read_queue_out before the actual read.
 *         (2) if it's RW WW WR model,call the read_queue_out after the actual read.
 */
u32 read_queue_out(const char* filename,IO_Type io_type,time_t arrive_time)
{
    Meta_Data *meta_data=(Meta_Data*)malloc(sizeof(Meta_Data));
    if(md_get(filename,meta_data)==0)
    {
        u32 head=(*meta_data).ioq.head;
        (*meta_data).ioq.head=(head+1)%IO_Q_LEN;//change the head
        (*meta_data).stat_info.st_atime=arrive_time;//change the access time
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
/*
 * Used to do the write queue out,It's should be called after the actual write operation.
 * Parameters:offset--come from the actual write operation. put it in the message to the replicas
 *            data--the written data
 *            size--the data size
 * Retruns:1--wrong 0--ok
 * Attentio: how to pass the written data to the replicas?
 *           we use the TT to store the data.
 */
u32 write_queue_out(const char* filename,IO_Type io_type,time_t arrive_time,u64 offset,char *data,size_t size)
{
    //path is the key to put the data in the TT
    char path[IO_NODE_KEY_LEN];
    sprintf(path, "%s_%ld", filename, arrive_time); 
    
    Meta_Data *meta_data=(Meta_Data*)malloc(sizeof(Meta_Data));
    if(md_get(filename,meta_data)==0)
    {
        //modify ioq head and modification time、size in metadata
        //In the actual write, you do not need to change the metadata.
        u32 head=(*meta_data).ioq.head;
        (*meta_data).ioq.head=(head+1)%IO_Q_LEN;
        (*meta_data).stat_info.st_mtime=arrive_time;
        (*meta_data).stat_info.st_size+=size;
        
        /*to add a new  io_node*/
        IO_Node *io_node=(IO_Node*)malloc(sizeof(IO_Node));
        (*io_node).io_type=io_type;
        (*io_node).offset=offset;
        (*io_node).io_time=arrive_time;
        strcpy((*io_node).ion_pre,(*meta_data).io_node_q_tail);//modify the pre pointer.
        strcpy((*io_node).ion_next," ");//next is NULL
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
                strcpy((*io_node1).ion_next,path);//use path to be the key of the IO_Node.
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
            strcpy((*meta_data).io_node_q_head,path);// if there is no IO_Node,we should change the head.
            u32 i, replica_num=(*meta_data).replica_num;
            for(i=0;i<replica_num;i++)
            {
                strcpy((*meta_data).my_rep[i].io_node_ptr,path);//After you update the replica,you should move the io_node_ptr to next.
            }
        }
        strcpy((*meta_data).io_node_q_tail,path);// modify the tail
        if(md_put(filename,meta_data)!=0)
        {
            printf("Put metadata in write_queue_out error!\n");
            return 1;
        }
        //free(meta_data);
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
    if(md_get(filename,meta_data)==0)
    {
        u32 i,replica_num=(*meta_data).replica_num;
        u8* host_ip;
        pid_t pid;
        for(i=0;i<replica_num;i++)
        {   
            strcpy(host_ip,(*meta_data).my_rep[i].host_ip);
        	
            // init a new process to deal with the message.
            if((pid=fork())==0)
            {
                SOCK_MSG sockmsg;
	            RPL_MSG rplmsg;
                sockmsg.type=SOCKMSG_TYPE_WRITE;
                strcpy(sockmsg.file_name,filename);
                strcpy(sockmsg.dest_ip,host_ip);
                printf("child pid:%d\n",getpid());
	            if(sctp_send_sock_recv_rpl(sockmsg.dest_ip,&sockmsg,&rplmsg) != 0){
		            fprintf(stderr,"sctp_send_sock_recv_rpl fail!\n");
		            return 1;
	                }
	            if(rplmsg.err != RPL_OK){
		            fprintf(stderr,"rplmsg says some err happened!\n");
		            return 1;
	                }
            }          
        }
        free(meta_data);
    }
    else
    {
        printf("Get metadata in write_queue_out error!\n");
        return 1;
    }
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
