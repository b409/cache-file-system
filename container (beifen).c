#include "container.h"
#include "request_analysis.h"

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
  const char *page = "hello, clound store!";
  struct MHD_Response *response;
  //int ret;
  
  if (strcmp(method,"GET") == 0)
  {
    //SimpleLog_Write(SL_DEBUG, __func__, "New %s request for %s using version %s", method, url, version);
	printf("%s New %s request for %s using version %s",__func__, method, url, version);
    request_get(cls,  connection, url,  method, version,  upload_data, 
		upload_data_size, con_cls);
	return 1;
  }    
  else if (strcmp(method,"PUT") == 0)
  {
    //SimpleLog_Write(SL_DEBUG, __func__, "New %s request for %s using version %s", method, url, version);
	printf("%s New %s request for %s using version %s",__func__, method, url, version);
    request_put( cls,  connection, url,  method, version,  upload_data, 
		upload_data_size,con_cls);
	return 1;
  }
  else if (strcmp(method,"DELETE") == 0)
  {
    //SimpleLog_Write(SL_DEBUG, __func__, "New %s request for %s using version %s", method, url, version);
	printf("%s New %s request for %s using version %s",__func__, method, url, version);    
	request_delete( cls,  connection, url,  method, version,  upload_data, 
		upload_data_size,con_cls);
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
    //SimpleLog_Write(SL_DEBUG, __func__, "New %s request for %s using version %s", method, url, version);
	printf("%s New %s request for %s using version %s",__func__, method, url, version);
    request_post( cls,  connection, url,  method, version,  upload_data, 
		upload_data_size, con_cls);
	return 1;
  }
  else
    //SimpleLog_Write(SL_ERROR, __func__, "Error %s request for %s using version %s", method, url, version);
	printf("%s ERROR %s request for %s using version %s",__func__, method, url, version);
  /*MHD_get_connection_values (connection, MHD_HEADER_KIND, print_out_key,
                             NULL);*/

/*the following is unused now*/
/******************************************************************/
/*
  response =
    MHD_create_response_from_data (strlen (page), (void *) page, MHD_NO,MHD_NO);
  ret = MHD_add_response_header(response, CDMI_VERSION_1_KEY, CDMI_VERSION_1_VALUE);
  //ret = ret ||(MHD_add_response_header(response, const char * header, const char * content))

  get_object_header_yes(response,send_header);
  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);
*/
  return 0;
}

int
main ()
{
  //SimpleLog_Setup(NULL, "%H:%M:%S", 0, 1, 0, "\t");
  //printf("%s New %s request for %s using version %s",__func__, method, url, version);
  struct MHD_Daemon *daemon;

  daemon = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION, PORT, NULL, NULL,
                             &answer_to_connection, NULL, MHD_OPTION_END);
  if (NULL == daemon)
	{   
		printf("start daemon failed!\n");		
		 return 1;
	}
  getchar ();

  MHD_stop_daemon (daemon);
  return 0;
}

