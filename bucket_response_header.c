#include "bucket_response_header.h"
#include "container.h"
#include "object_response_header.h"
/*
int get_bucket_header_yes(struct MHD_Response *response,struct sender_head send_header)
{

  //  

	char *header_add;
	struct stat buf;
	char *temp;
	temp=(char *)malloc(BUFLENGTH*sizeof(char));
	//header_add=(char *)malloc(BUFLENGTH*sizeof(char));
	header_add = MHD_get_response_header (response,"Date");
	if (header_add != NULL)
	{
		if(Gen_Date(header_add)==0)
			err_sys("get_object_header_yes Gen_Date error");

	}
	else
	{

		if(Gen_Date(header_add)==0)
			err_sys("get_object_header_yes Gen_Date error");
		if(MHD_add_response_header(response,"Date", header_add)==0)
		{
			//printf("%s\n",header_add);			
			err_sys("get_object_header_yes MHD_add_response_header Date error");
		}
	}
	header_add = MHD_get_response_header (response,"Last-Modified");	
	if(stat(send_header.Pathname,&buf)<0)
			err_sys("get_object_header_yes stat error");
	else
	{
			 if(Tran_Date(buf.st_mtime,temp)==0)
			 	err_sys("get_object_header_yes Tran_Date error");
	}

	if (header_add != NULL)
	{
		 strncpy(header_add,temp,strlen(temp)+1);
			
	}
	else
	{
		if(MHD_add_response_header(response,"Last-Modified", temp)==0)
			err_sys("get_object_header_yes MHD_add_response_header Last-Modified error");

	}
	
	header_add = MHD_get_response_header (response,"Content-Length");	
	//ltoa(buf.st_size,temp,10);
	temp=my_itoa(buf.st_size);
	if (header_add != NULL)
	{
			
			 	//err_sys("get_object_header_yes Tran_Date error");
			 strncpy(header_add,temp,strlen(temp)+1);
			
	}
	else
	{
		if(MHD_add_response_header(response,"Content-Length", temp)==0)
			err_sys("get_object_header_yes MHD_add_response_header Content-Length error");

	}


	header_add = MHD_get_response_header (response,"ETag");	
	if (header_add != NULL)
	{
		//header_add=NULL:
		strncpy(header_add,"\"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\"",strlen("\"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\"")+1);
		}
	else
	{
		if(MHD_add_response_header(response,"ETag", "\"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\"")==0)
			err_sys("get_object_header_yes MHD_add_response_header ETag error");

	}

		header_add = MHD_get_response_header (response,"Connection");	
			// ltoa(buf.st_size,temp,10);
			if (header_add != NULL)
			{
					
						//err_sys("get_object_header_yes Tran_Date error");
					 strncpy(header_add,send_header.Connection,strlen(send_header.Connection)+1);
					
			}
			else
			{
				if(MHD_add_response_header(response,"Connection", send_header.Connection)==0)
					err_sys("get_object_header_yes MHD_add_response_header Connection error");
		
			}
header_add = MHD_get_response_header (response,"Content-Type");	
		//ltoa(buf.st_size,temp,10);
		if (header_add != NULL)
		{
				
					//err_sys("get_object_header_yes Tran_Date error");
				 strncpy(header_add,"text/plain",strlen("text/plain")+1);
				
		}
		else
		{
			if(MHD_add_response_header(response,"Content-Length", "text/plain")==0)
				err_sys("get_object_header_yes MHD_add_response_header Content-Length error");
	
		}

		
		header_add = MHD_get_response_header (response,"Server"); 
				//ltoa(buf.st_size,temp,10);
		if (header_add != NULL)
		{
						
							//err_sys("get_object_header_yes Tran_Date error");
			 strncpy(header_add,"Store B409",strlen("Store B409")+1);
						
		}
			else
		{
			if(MHD_add_response_header(response,"Server", "Store B409")==0)
				err_sys("get_object_header_yes MHD_add_response_header Content-Length error");
			
		}
		
		return 1;
	}


int get_bucket_header_no(struct MHD_Response *response,struct sender_head send_header)
{
//

	char *header_add;
	struct stat buf;
	char *temp;
	temp=(char *)malloc(BUFLENGTH*sizeof(char));
	//header_add=(char *)malloc(BUFLENGTH*sizeof(char));
	header_add = MHD_get_response_header (response,"Date");
	if (header_add != NULL)
	{
		if(Gen_Date(header_add)==0)
			err_sys("get_object_header_no Gen_Date error");

	}
	else
	{

		if(Gen_Date(header_add)==0)
			err_sys("get_object_header_no Gen_Date error");
		if(MHD_add_response_header(response,"Date", header_add)==0)
			err_sys("get_object_header_no MHD_add_response_header Date error");

	}
		header_add = MHD_get_response_header (response,"Connection");	
			// ltoa(buf.st_size,temp,10);
	if (header_add != NULL)
			{
					
						//err_sys("get_object_header_yes Tran_Date error");
					 strncpy(header_add,send_header.Connection,strlen(send_header.Connection)+1);
					
			}
			else
			{
				if(MHD_add_response_header(response,"Connection", send_header.Connection)==0)
					err_sys("get_object_header_no MHD_add_response_header Connection error");
		
			}

		if(MHD_add_response_header(response,"Server", "Store B409")==0)
				err_sys("get_object_header_no MHD_add_response_header Content-Length error");
			
	
		

	
		header_add = MHD_get_response_header (response,"Server"); 
				//ltoa(buf.st_size,temp,10);
		if (header_add != NULL)
		{
						
							//err_sys("get_object_header_no Tran_Date error");
			 strncpy(header_add,"Store B409",strlen("Store B409")+1);

			}
		else
		{
			if(MHD_add_response_header(response,"x-amz-delete-marker", "TRUE")==0)
				err_sys("get_object_header_no MHD_add_response_header x-amz-delete-marker error");
			
		}
		
		return 1;
	}






*/

















