#include "request_analysis.h"
#include "container.h"
#include "object_response_header.h"
#include "xml.h"
#include "bucket_response_header.h"


int
request_get(void *cls, struct MHD_Connection *connection,          
                      const char *url, const char *method,
                      const char *version,const char *upload_data,
                      size_t *upload_data_size, void **con_cls)
{

	char pathname[128];
		//char bucket[64];
		char b_host[128];
		char *temp;
		char *pathname_xml;
		char *delim=".";
		int n,m,i,ret,fd;
		struct stat sbuf;;
		struct MHD_Response *response;
		int count[1];
		char buftest[BUFTESTSIZE];
		sender_head send_header;
	
	
		temp=MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Host");
		printf("request_analysis 81 line host is %s\n",temp);
		strncpy(b_host,temp,strlen(temp)+1);
		strtok(b_host, delim);
		strncpy(pathname,b_host,strlen(b_host)+1);
		//strncpy(pathname_xml,b_host,strlen(b_host)+1);
		//strcat(pathname_xml,".xml");
		pathname_xml=my_itoa(getpid());
	
  	//if(strcmp(url,"/")==0) {
	//************************judge get objects or bucket********
	printf("url is %s\n",url);
	//************************sender_head 

	strncpy(send_header.Connection,"close",strlen("close")+1);
	printf("request_analysis.c 95line  date is %s \n",send_header.Connection);

	strncpy(send_header.Pathname,url,strlen(url)+1);
	printf("request_analysis.c 98 line  url is %s \n",send_header.Pathname);
	//***************************sender_head end

	if(is_dir_xml(url)){
	

	printf("%s List the Contents of  bucket\n: %s",__func__, url);
	creat_xml(pathname_xml);
	printf("\n pathname : %s\n",pathname);
	



	//is_dir_xml(const char *path)
	delete_file_xml(url,count,pathname_xml);
	
	//******************test http header********************** 
	temp=MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Date");
	if(NULL == temp)
		printf("GET GET request_analysis.c 116 line send package date is NULL\n");
	else
		printf("GET GET request_analysis.c 172 line send package date: %s \n",temp);

	
	//*********************end test http header**************
	

	if ( (-1 == (fd = open (pathname_xml, O_RDONLY))) ||(0 != fstat (fd, &sbuf)) )
	{
	/* error accessing file */
		if (fd != -1) 
			close (fd);
		const char *errorstr ="<html><body>error accessing bucket has occured!</body></html>";
		response =MHD_create_response_from_buffer (strlen (errorstr),(void *) errorstr,MHD_RESPMEM_PERSISTENT);
		if (response)
		{

			get_bucket_header_no(response,send_header);
			ret =MHD_queue_response (connection, MHD_HTTP_INTERNAL_SERVER_ERROR,response);
				
			MHD_destroy_response (response);
			return MHD_YES;
		}
		
		else
				return MHD_NO;
	}
	response =MHD_create_response_from_fd_at_offset (sbuf.st_size, fd, 0);
	if(response)
	{
		//MHD_add_response_header (response, "Content-Type", MIMETYPE);
		get_bucket_header_yes(response,send_header);
		ret = MHD_queue_response (connection, MHD_HTTP_OK, response);

			MHD_destroy_response (response);
		return MHD_YES;
	}
	else
		return MHD_NO;
//}
	//printf("%s List the Contents of  Container: %s",__func__, url);
}
//***************************************get object************	
  else 
  {
    

	printf("%s Get object: %s\n", __func__,url);
	
	printf("PATHNAME is %s,URL is %s\n",pathname,url);

	if ( (-1 == (fd = open (url, O_RDONLY))) ||(0 != fstat (fd, &sbuf)) )
	{
	
		if (fd != -1) 
			close (fd);
		const char *errorstr ="<html><body>error accessing file has occured!<br>access the error file!</body></html>";
		printf("182 line *************The file %s is not exist\n",url);
		response =MHD_create_response_from_buffer (strlen(errorstr),(void *)errorstr,MHD_RESPMEM_PERSISTENT);
		if (response)
		{

				get_object_header_no(response,send_header);
			ret =MHD_queue_response (connection, MHD_HTTP_INTERNAL_SERVER_ERROR,response);
				
			MHD_destroy_response (response);
			return MHD_YES;
		}
		
		else
				return MHD_NO;
	}
	printf("*************196 line create response**************\n");

	response =MHD_create_response_from_fd_at_offset (sbuf.st_size, fd, 0);
	if(response)
	{
		//MHD_add_response_header (response, "Content-Type", MIMETYPE);
		get_object_header_yes(response,send_header);
		ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
		MHD_destroy_response (response);
		return MHD_YES;
	}
	else
		return MHD_NO;


//}

}
return 1;
}


int     
request_put(void *cls, struct MHD_Connection *connection,         \
                      const char *url, const char *method,         \
                      const char *version,const char *upload_data,   \
                      size_t *upload_data_size, void **con_cls)
{

		char pathname[128];
		char bucket[64];
		char b_host[128];
		char *temp;
		char *pathname_xml;
		char *delim=".";
		int n,m,i,ret,fd;
		struct stat sbuf;;
		struct MHD_Response *response;
		int count[1];
		char buftest[BUFTESTSIZE];
		sender_head send_header;
		//*****************add to generate a bucket or object name
		char *bucket_object[128];
	
	
		temp=MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Host");
		printf("request_analysis 414 line host is %s\n",temp);
		strncpy(b_host,temp,strlen(temp)+1);
		strtok(b_host, delim);
		strncpy(pathname,b_host,strlen(b_host)+1);
		//strncpy(pathname_xml,b_host,strlen(b_host)+1);
		//strcat(pathname_xml,".xml");
		//POST pathname_xml=my_itoa(getpid());
	
  	//if(strcmp(url,"/")==0) {
	//************************judge get objects or bucket********
	printf("url is %s\n",url);
	//************************sender_head 

	strncpy(send_header.Connection,"close",strlen("close")+1);
	printf("request_analysis.c 428 line  date is %s \n",send_header.Connection);

	strncpy(send_header.Pathname,url,strlen(url)+1);
	printf("request_analysis.c 431 line  url is %s \n",send_header.Pathname);
	//***************************sender_head end



	get_sonstr(url,bucket_object);
	if(judge_object_or_bucket_in_put(bucket_object)>0)
	{
	//***********************POST bucket***********************
		printf("PUT a bucket named %s\n",bucket_object);
		if(mkdir(url,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)<0)
		{		
			printf("in PUT create bucket %s error !\n",bucket_object);
			return 0;
		}
		response =MHD_create_response_from_buffer(0,NULL, MHD_RESPMEM_PERSISTENT);
		if(response)
		{
					
			put_bucket_header_yes(response,send_header);	
			ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
			MHD_destroy_response (response);
			return MHD_YES;
		}
		else
			return MHD_NO;

	}		
	

		
	else{






	printf("%s PUT  data: %s",__func__, url);
	
	temp=MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Host");
	
	strncpy(b_host,temp,strlen(temp)+1);
	strtok(b_host, delim);
	strncat(bucket,b_host,strlen(b_host)+1);

	if(chdir(bucket)<0)
	{
		perror("chdir error");
		return 0;
	}
	


	
	

	
		if (NULL == *con_cls)

		{
			struct connection_info_struct *con_info;
			//if (nr_of_uploading_clients >= MAXCLIENTS)
				//return send_page (connection, busypage, MHD_HTTP_SERVICE_UNAVAILABLE);
			con_info = malloc (sizeof (struct connection_info_struct));
			if (NULL == con_info)
				return MHD_NO;
			con_info->fp = NULL;
			
			con_info->postprocessor =
					MHD_create_post_processor (connection, POSTBUFFERSIZE,iterate_post, (void *) con_info);
			if (NULL == con_info->postprocessor)
			{
				free (con_info);
				return MHD_NO;
			}
				//nr_of_uploading_clients++;
			con_info->connectiontype = POST;
			con_info->answercode = MHD_HTTP_OK;
			con_info->answerstring = completepage;
		}
			/*else
				con_info->connectiontype = GET;
				*con_cls = (void *) con_info;
				return MHD_YES;
		}                                            
		if (0 == strcmp (method, "GET"))
		{
			int ret;
			char buffer[1024];
			sprintf (buffer, askpage, nr_of_uploading_clients);
			return post_send_page (connection, buffer, MHD_HTTP_OK);
		}*/
		
		struct connection_info_struct *con_info = *con_cls;
		if (0 != *upload_data_size)
		{
			MHD_post_process (con_info->postprocessor, upload_data,*upload_data_size);
			*upload_data_size = 0;



			put_request_completed (cls, connection,con_cls,MHD_REQUEST_TERMINATED_COMPLETED_OK);
			
			return MHD_YES;

		}
		else
			return put_send_page (connection, con_info->answerstring,con_info->answercode);

	return put_send_page (connection, errorpage, MHD_HTTP_BAD_REQUEST);
}
return 1;

}



int
request_delete(void *cls, struct MHD_Connection *connection,         \
                      const char *url, const char *method,         \
                      const char *version,const char *upload_data,   \
                      size_t *upload_data_size, void **con_cls)
{

		char pathname[128];
		//char bucket[64];
		char b_host[128];
		char *temp;
		char *pathname_xml;
		char *delim=".";
		int n,m,i,ret,fd;
		struct stat sbuf;;
		struct MHD_Response *response;
		int count[1];
		char buftest[BUFTESTSIZE];
		sender_head send_header;
	
	
		temp=MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Host");
		printf("request_analysis 81 line host is %s\n",temp);
		strncpy(b_host,temp,strlen(temp)+1);
		strtok(b_host, delim);
		strncpy(pathname,b_host,strlen(b_host)+1);
		//strncpy(pathname_xml,b_host,strlen(b_host)+1);
		//strcat(pathname_xml,".xml");
		pathname_xml=my_itoa(getpid());
	
  	//if(strcmp(url,"/")==0) {
	//************************judge get objects or bucket********
	printf("url is %s\n",url);
	//************************sender_head 

	strncpy(send_header.Connection,"close",strlen("close")+1);
	printf("request_analysis.c 95line  date is %s \n",send_header.Connection);

	strncpy(send_header.Pathname,url,strlen(url)+1);
	printf("request_analysis.c 98 line  url is %s \n",send_header.Pathname);
	//***************************sender_head end

	if(is_dir_xml(url)){


  
    //SimpleLog_Write(SL_DEBUG, __func__, "Delete a Container: %s", url);
	printf("459 line %s Delete a bucket: %s", __func__,url);
	delete_file(url);
	delete_dir(url);
	
	response =MHD_create_response_from_buffer(0,NULL, MHD_RESPMEM_PERSISTENT);
			if(response)
			{
							//MHD_add_response_header (response, "Content-Type", MIMETYPE);
				delete_bucket_header_yes(response,send_header);
	
					
				ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
				MHD_destroy_response (response);
				return MHD_YES;
			}
			else
				return MHD_NO;

  }
  else
  {

	
		printf("482 line %s Delete object: %s",__func__, url);
		

		if(unlink(url)<0)
		{
			perror("490 line DELETE request_delete unlink \n");
			return 0;
		}


		response =MHD_create_response_from_buffer(0,NULL, MHD_RESPMEM_PERSISTENT);
		if(response)
		{
						//MHD_add_response_header (response, "Content-Type", MIMETYPE);
			delete_object_header_yes(response,send_header);

				
			ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
			MHD_destroy_response (response);
			return MHD_YES;
		}
		else
			return MHD_NO;

		
  	
	}
	return 1;
}


int
request_head(void *cls, struct MHD_Connection *connection,         \
                      const char *url, const char *method,         \
                      const char *version,const char *upload_data,   \
                      size_t *upload_data_size, void **con_cls)
{

	char pathname[128];
		//char bucket[64];
		char b_host[128];
		char *temp;
		char *pathname_xml;
		char *delim=".";
		int n,m,i,ret,fd;
		DIR *dp;
		struct stat sbuf;;
		struct MHD_Response *response;
		int count[1];
		char buftest[BUFTESTSIZE];
		sender_head send_header;
	
	
		temp=MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Host");
		printf("645 line HEAD HEAD request_analysis 81 line host is %s\n",temp);
		strncpy(b_host,temp,strlen(temp)+1);
		strtok(b_host, delim);
		strncpy(pathname,b_host,strlen(b_host)+1);
		//strncpy(pathname_xml,b_host,strlen(b_host)+1);
		//strcat(pathname_xml,".xml");
	//  HEAD 	pathname_xml=my_itoa(getpid());
	
  	//if(strcmp(url,"/")==0) {
	//************************judge get objects or bucket********
	printf("url is %s\n",url);
	//************************sender_head 

	strncpy(send_header.Connection,"close",strlen("close")+1);
	printf("HEAD HEAD request_analysis.c 659 line  date is %s \n",send_header.Connection);

	strncpy(send_header.Pathname,url,strlen(url)+1);
	printf("HEAD HEAD request_analysis.c 662 line  url is %s \n",send_header.Pathname);
	//***************************sender_head end

	if(is_dir_xml(url)){
	

	printf("%s HEAD HEAD the attribute of  bucket\n: %s",__func__, url);
	// HEAD  creat_xml(pathname_xml);
	printf("\n pathname : %s\n",pathname);
	



	//is_dir_xml(const char *path)
	 // HEAD HEAD   delete_file_xml(url,count,pathname_xml);
	
	//******************test http header********************** 
	temp=MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Date");
	if(NULL == temp)
		printf("HEAD HEAD request_analysis.c 681 line  send package date is NULL\n");
	else
		printf("HEAD HEAD request_analysis.c 683 line get response header date: %s \n",temp);

	


	//open the dir to confirm whether it is esited

	if (NULL== (dp= opendir (url)))
	{
		const char *errorstr ="<html><body>HEAD The bucket not existed!</body></html>";
		response =MHD_create_response_from_buffer (strlen (errorstr),(void *) errorstr,MHD_RESPMEM_PERSISTENT);
		
		if(closedir(dp)<0)
			printf("IN HEAD close dir error!\n");		


		if (response)
		{

			head_bucket_header_no(response,send_header);
			ret =MHD_queue_response (connection, MHD_HTTP_INTERNAL_SERVER_ERROR,response);
				
			MHD_destroy_response (response);
			return MHD_YES;
		}
		
		else
				return MHD_NO;
	}
	else
	{
		response =MHD_create_response_from_buffer(0,NULL, MHD_RESPMEM_PERSISTENT);
				if(response)
					{
						//MHD_add_response_header (response, "Content-Type", MIMETYPE);
						head_bucket_header_yes(response,send_header);

				
						ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
						MHD_destroy_response (response);
						return MHD_YES;
					}
				else
					return MHD_NO;

	}		
	
}


//**************************************************************************************
//***************************************Head object************	
  else 
  {
    //SimpleLog_Write(SL_DEBUG, __func__, "Get the Contents of  data: %s", url);
    //goto data function
	

	printf("%s HEAD HEAD object: %s\n", __func__,url);
	
	printf("PATHNAME is %s,URL is %s\n",pathname,url);

	if (0!= fstat (fd, &sbuf))
	{
	

		const char *errorstr ="<html><body>error accessing file has occured!<br>access the error file!</body></html>";
		printf("816 HEAD HEAD  line *************The file %s is stat error\n",url);
		response =MHD_create_response_from_buffer (strlen(errorstr),(void *)errorstr,MHD_RESPMEM_PERSISTENT);
		if (response)
		{
				head_object_header_no(response,send_header);
			ret =MHD_queue_response (connection, MHD_HTTP_INTERNAL_SERVER_ERROR,response);
				
			MHD_destroy_response (response);
			return MHD_YES;
		}
		
		else
				return MHD_NO;
	}
	printf("*************HEAD HEAD 830 line create response**************\n");
	//*****************************************************
	//*****************************************************
	//*****************************************************	
/*
	while((n=read(fd,buftest,BUFTESTSIZE))>0)
		if(write(STDOUT_FIFENO,buftest,n)!=n)
			printf("*********write error*************\n");
	if(n<0)
		printf("***************read error**********\n");
*/
	response =MHD_create_response_from_buffer(0,NULL, MHD_RESPMEM_PERSISTENT);
	if(response)
	{
		//MHD_add_response_header (response, "Content-Type", MIMETYPE);
		head_object_header_yes(response,send_header);
		ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
		MHD_destroy_response (response);
		return MHD_YES;
	}
	else
		return MHD_NO;


//}

}
return 1;


}




//=======================the end of the new version =====================================
//***************************************************************************************************


int
request_post(void *cls, struct MHD_Connection *connection,         \
                      const char *url, const char *method,         \
                      const char *version,const char *upload_data,   \
                      size_t *upload_data_size, void **con_cls)
{
  
	char pathname[128];
		char bucket[64];
		char b_host[128];
		char *temp;
		char *pathname_xml;
		char *delim=".";
		int n,m,i,ret,fd;
		struct stat sbuf;;
		struct MHD_Response *response;
		int count[1];
		char buftest[BUFTESTSIZE];
		sender_head send_header;
		//*****************add to generate a bucket or object name
		char *bucket_object[128];
	
	
		temp=MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Host");
		printf("request_analysis 934 line host is %s\n",temp);
		strncpy(b_host,temp,strlen(temp)+1);
		strtok(b_host, delim);
		strncpy(pathname,b_host,strlen(b_host)+1);
		//strncpy(pathname_xml,b_host,strlen(b_host)+1);
		//strcat(pathname_xml,".xml");
		//POST pathname_xml=my_itoa(getpid());
	
  	//if(strcmp(url,"/")==0) {
	//************************judge get objects or bucket********
	printf("url is %s\n",url);
	//************************sender_head 

	strncpy(send_header.Connection,"close",strlen("close")+1);
	printf("request_analysis.c 95line  date is %s \n",send_header.Connection);

	strncpy(send_header.Pathname,url,strlen(url)+1);
	printf("request_analysis.c 98 line  url is %s \n",send_header.Pathname);
	//***************************sender_head end



	get_sonstr(url,bucket_object);
	if(judge_object_or_bucket_in_put(bucket_object)>0)
	{
	//***********************POST bucket***********************
		printf("POST a bucket named %s\n",bucket_object);
		if(mkdir(url,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)<0)
		{		
			printf("create bucket %s error !\n",bucket_object);
			return 0;
		}
		response =MHD_create_response_from_buffer(0,NULL, MHD_RESPMEM_PERSISTENT);
		if(response)
		{
					
			post_bucket_header_yes(response,send_header);	
			ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
			MHD_destroy_response (response);
			return MHD_YES;
		}
		else
			return MHD_NO;

	}		
	

		
	else{
		printf("%s Post  data: %s",__func__, url);
		



	//char *cheek;
  if (NULL == *con_cls)
    {
      struct connection_info_struct *con_info;

      if (nr_of_uploading_clients >= MAXCLIENTS)
        return send_page (connection, busypage, MHD_HTTP_SERVICE_UNAVAILABLE);

      con_info = malloc (sizeof (struct connection_info_struct));
      if (NULL == con_info)
        return MHD_NO;

      con_info->fp = NULL;
	printf("method is %s\n",method);
		printf("in answer url is %s\n",url);
	//	printf("version is %s\n",version);
	//	printf("upload_data  is  %s\n",upload_data);
	//	printf("*upload_data_size is  %d\n",*upload_data_size);

	//	cheek=MHD_lookup_connection_value(connection, MHD_POSTDATA_KIND, "user");
		
	//	printf("cheek  is  %s \n",cheek);


      if (0 == strcmp (method, "POST"))
        {
          con_info->postprocessor =MHD_create_post_processor (connection, POSTBUFFERSIZE,
                                       iterate_post, (void *) con_info);

          if (NULL == con_info->postprocessor)
            {
              free (con_info);
		printf("MHD_create_post_processor error!\n");
              return MHD_NO;
            }

          nr_of_uploading_clients++;

          con_info->connectiontype = POST;
          con_info->answercode = MHD_HTTP_OK;
          con_info->answerstring = completepage;
        }
      else
        con_info->connectiontype = GET;

      *con_cls = (void *) con_info;

      return MHD_YES;
    }

  if (0 == strcmp (method, "GET"))
    {
      char buffer[1024];

      snprintf (buffer, sizeof (buffer), askpage, nr_of_uploading_clients);
      return send_page (connection, buffer, MHD_HTTP_OK);
    }

  if (0 == strcmp (method, "POST"))
    {
      struct connection_info_struct *con_info = *con_cls;

      if (0 != *upload_data_size)
        {
          MHD_post_process (con_info->postprocessor, upload_data,
                            *upload_data_size);
          *upload_data_size = 0;

          return MHD_YES;
        }
      else
	{
	  if (NULL != con_info->fp)
		{
	    fclose (con_info->fp);
		 con_info->fp = NULL; 

	}
	  /* Now it is safe to open and inspect the file before calling send_page with a response */
	  return send_page (connection, con_info->answerstring,
			    con_info->answercode);
	}

    }

  return send_page (connection, errorpage, MHD_HTTP_BAD_REQUEST);
}

}

