#include "container.h"
#include "request_analysis.h"
#include "xml.h"
#include "glob.h"
#include "md_type.h"
#include "io_queue.h"

#define PORT 8888
/*
int
print_out_key (void *cls, enum MHD_ValueKind kind, const char *key,
               const char *value)
{
  SimpleLog_Write(SL_DEBUG,  __func__, "%s: %s", key, value);
  return MHD_YES;
}
*/
int
answer_to_connection (void *cls, struct MHD_Connection *connection,
                      const char *url, const char *method,
                      const char *version, const char *upload_data,
                      size_t *upload_data_size, void **con_cls)
{
  /*the info of this thread, so we need there to be defined in function, 
     not be defined as global variables.
  */
 // json_object * Response_page = json_object_new_object();
  //json_object * Response_heads = json_object_new_object();
  //const char *page = "hello, cloud store!";
  struct MHD_Response *response;
	//puts(page);
  //int ret;
char pathname[128];
char pathname_mnt[128];
get_sonstr(url,pathname);
memset(pathname_mnt,0,sizeof(pathname_mnt));
strcat(strcat(pathname_mnt,"/mnt/supercache/"),pathname);

    //printf("pathname %s     pathname_mnt  %s  \n",pathname,pathname_mnt);

  if (strcmp(method,"GET") == 0)
  {
    printf("haha\n");
    //SimpleLog_Write(SL_DEBUG, __func__, "New %s request for %s using version %s", method, url, version);
	printf("%s New %s request for %s using version %s",__func__, method, url, version);
   /******************add by Jin*********************************/ 
    /*time_t arrive_time;
    time(&arrive_time);
    IO_Type io_type=READ;
    queue_in_wait(pathname_mnt,io_type,arrive_time);
    int already_queue_out=0;//the mark of read out early

     Meta_Data * meta_data=(Meta_Data*)malloc(sizeof(Meta_Data));
     if(md_get(pathname_mnt,meta_data)==0)
     {
         u32 head_next=((*meta_data).ioq.head+1)%IO_Q_LEN;
         //if one read comes after one read ,queue out early
         if((*meta_data).ioq.io_q_node[head_next].io_type==READ)
         {
             read_queue_out(pathname_mnt,io_type,arrive_time);
             already_queue_out=1;
         }
     }*/
     /*******************************************************/
    request_get(cls,  connection, url,  method, version,  upload_data, 
		upload_data_size, con_cls);
    
    /* if(already_queue_out==0)
     {
         read_queue_out(pathname_mnt,io_type,arrive_time);
     }*/
	return 1;
  }    
  else if (strcmp(method,"PUT") == 0)
  {
    //SimpleLog_Write(SL_DEBUG, __func__, "New %s request for %s using version %s", method, url, version);
	printf("%s New %s request for %s using version %s",__func__, method, url, version);

    /******************************************************/
    /*time_t arrive_time;
    time(&arrive_time);
    IO_Type io_type=WRITE;
    queue_in_wait(pathname_mnt,io_type,arrive_time);*/
   /********************************************************/
    request_put( cls,  connection, url,  method, version,  upload_data, 
		upload_data_size,con_cls);
   /*********************************************************/ 
    /*u64 offset=0;char *data="never mind!";size_t size1=11;
    write_queue_out(pathname_mnt,io_type,arrive_time,offset,data,size1);//should change it*/
    /*********************************************************/
	return 1;
  }
  else if (strcmp(method,"DELETE") == 0)
  {
    //SimpleLog_Write(SL_DEBUG, __func__, "New %s request for %s using version %s", method, url, version);
	printf("%s New %s request for %s using version %s",__func__, method, url, version);    
    /************************************************/
   /* time_t arrive_time;
    time(&arrive_time);
    IO_Type io_type=REMOVE;
    queue_in_wait(pathname_mnt,io_type,arrive_time);*/
    /************************************************/
	request_delete( cls,  connection, url,  method, version,  upload_data, 
		upload_data_size,con_cls);
    //remove_queue_out(pathname_mnt,io_type);
  }
  else if (strcmp(method,"HEAD") == 0)
  {
    //SimpleLog_Write(SL_DEBUG, __func__, "New %s request for %s using version %s", method, url, version);
	printf("%s New %s request for %s using version %s",__func__, method, url, version);
    request_head( cls,  connection, url,  method, version,  upload_data, 
		upload_data_size,con_cls);
	return 1;
  }

 else if (strcmp(method,"POST") == 0)
  {
      //printf("()()()()\n");
    //SimpleLog_Write(SL_DEBUG, __func__, "New %s request for %s using version %s", method, url, version);
	//printf("%s New %s request for %s using version %s\n",__func__, method, url, version);
    //printf("up_load_size:%u  %s\n",upload_data_size,upload_data);
    /******************************************************/
    /*time_t arrive_time;
    time(&arrive_time);
    IO_Type io_type=WRITE;
    queue_in_wait(pathname_mnt,io_type,arrive_time);*/
   /********************************************************/
    request_post(cls,  connection, url,  method, version,  upload_data, 
		upload_data_size, con_cls);
   /*********************************************************/ 
    /*u64 offset=0;char *data="never mind!";size_t size1=11;
    write_queue_out(pathname_mnt,io_type,arrive_time,offset,data,size1);//should change it*/
    /*********************************************************/
	return 1;
  }
  else
    
	printf("%s ERROR %s request for %s using version %s",__func__, method, url, version);

  return 0;
}

void request_completed(void* cls,struct MHD_Connection *connection, 
                       void **con_cls,enum MHD_RequestTerminationCode toe)
{
    printf("**********************request completted**************************\n");
}

int main ()
{
  //SimpleLog_Setup(NULL, "%H:%M:%S", 0, 1, 0, "\t");
  //printf("%s New %s request for %s using version %s",__func__, method, url, version);
  struct MHD_Daemon *daemon;
 // unsigned int nr_of_uploading_clients=0;
  daemon = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION, PORT, NULL, NULL,
                             &answer_to_connection, NULL, MHD_OPTION_NOTIFY_COMPLETED,
                             &request_completed,NULL,MHD_OPTION_END);
  if (NULL == daemon)
	{   
		printf("start daemon failed!\n");		
		 return 1;
	}
  getchar ();

  MHD_stop_daemon (daemon);
  return 0;
}

