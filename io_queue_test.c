#include"io_queue.h"

int main()
{
    Meta_Data * meta_data=(Meta_Data*)malloc(sizeof(Meta_Data));
    meta_data->ioq.tail=0;
    meta_data->ioq.head=0;
    char * filename="lollipop";
   if( md_put(filename,meta_data)!=0)
    {
        printf("Put in main error!\n");
    }
    time_t now_time;
    time(&now_time);
    IO_Type io_type=READ;
    queue_in_wait(filename,io_type,now_time);
    /*test*/
    md_get(filename,meta_data);
    int head=(*meta_data).ioq.head;
    int tail=(*meta_data).ioq.tail;
    printf("%d---%d\n",head,tail);
    printf("%ld---%d\n",(*meta_data).ioq.io_q_node[tail-1].arrive_time,(*meta_data).ioq.io_q_node[tail-1].io_type);



    read_queuue_out(filename,io_type,now_time);

    io_type=WRITE;
    queue_in_wait(filename,io_type,now_time);
    md_get(filename,meta_data);
    head=(*meta_data).ioq.head;
    tail=(*meta_data).ioq.tail;
    printf("%d---%d\n",head,tail);
    printf("%ld---%d\n",(*meta_data).ioq.io_q_node[tail-1].arrive_time,(*meta_data).ioq.io_q_node[tail-1].io_type);
    printf("%ld\n",(*meta_data).stat_info.st_atime);
    char *data="hello world.dsaddas ";
    u64 offset=0;
    size_t size=strlen(data);
    write_queue_out(filename,io_type,now_time,offset,data,size);
    md_get(filename,meta_data);
    head=(*meta_data).ioq.head;
    tail=(*meta_data).ioq.tail;
    printf("%d---%d\n",head,tail);
    printf("%s--%s\n",(*meta_data).io_node_q_head,(*meta_data).io_node_q_tail);

    IO_Node *io_node=(IO_Node*)malloc(sizeof(IO_Node));
    ion_get((*meta_data).io_node_q_head,io_node);
    printf("%s---%s\n",(*io_node).ion_pre,(*io_node).ion_next);

    
    io_type=WRITE;
    time_t time1;
    time(&time1);
    time1+=100000;
    char path[IO_NODE_KEY_LEN];
    sprintf(path,"%s_%ld",filename,time1);
    queue_in_wait(filename,io_type,time1);
    md_get(filename,meta_data);
    head=(*meta_data).ioq.head;
    tail=(*meta_data).ioq.tail;
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!%d---%d\n",head,tail);
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!%ld---%d\n",(*meta_data).ioq.io_q_node[tail-1].arrive_time,(*meta_data).ioq.io_q_node[tail-1].io_type);
    printf("%ld\n",(*meta_data).stat_info.st_atime);
    char *data1="hello wddasdad dasdasd dasd ";
    size=strlen(data1);
    write_queue_out(filename,io_type,time1,offset,data1,size);
    md_get(filename,meta_data);
    head=(*meta_data).ioq.head;
    tail=(*meta_data).ioq.tail;
    printf("%d---%d\n",head,tail);
    printf("%s--%s\n",(*meta_data).io_node_q_head,(*meta_data).io_node_q_tail);

    ion_get((*meta_data).io_node_q_head,io_node);
    printf("%s---%s\n",(*io_node).ion_pre,(*io_node).ion_next);
    ion_get((*io_node).ion_next,io_node);
    printf("%s---%s\n",(*io_node).ion_pre,(*io_node).ion_next);
    int size_data;
    char*data2=iod_get(path,&size_data);
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%s-----%d\n",data2,size_data); 
    free(data2);
    return 0;

}
