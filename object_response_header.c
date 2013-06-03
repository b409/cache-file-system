#include "object_response_header.h"
#include "request_analysis.h"
#include "container.h"
#include "posix_for_s3.h"
#include "utility.h"


int get_object_header_yes(struct MHD_Response *response,struct sender_head send_header)
{

  
	char *header_add,*temp,*date;
	struct stat buf;
	temp=(char *)malloc(BUFLENGTH*sizeof(char));
	date=(char *)malloc(BUFLENGTH*sizeof(char));
	
    printf("send_header Pathname is %s  and  Connection is  %s !\n",send_header.Pathname,send_header.Connection);
    
    //header_add=(char *)malloc(BUFLENGTH*sizeof(char));
	header_add = MHD_get_response_header (response,"Date");
	if (header_add != NULL)
	{
		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)
			err_sys("get_object_header_yes Gen_Date error");		
		strncpy(header_add,date,strlen(date)+1);

	}
	else
	{

		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)
			err_sys("get_object_header_yes Gen_Date error");
		//printf("%s\n",header_add);
		printf("62 line date: %s\n",date);
	
		//if(MHD_add_response_header(response,"Date", header_add)==0)
		if(MHD_add_response_header(response,"Date", date)==0)
		{
			printf("%s\n",date);
			err_sys("get_object_header_yes MHD_add_response_header Date error");
		}
	}

	header_add = MHD_get_response_header (response,"Last-Modified");	
	if(stat(send_header.Pathname,&buf)<0)
			err_sys("get_object_header_yes stat 90 line error");
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
			printf("*********117 line size is %s\n",temp);	
	}
	else
	{
		printf("*********117 line size is %s\n",temp);				
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
      printf("take care!\n");
		header_add = MHD_get_response_header (response,"Connection");	
			// ltoa(buf.st_size,temp,10);
			if (header_add != NULL)
			{
					
						//err_sys("get_object_header_yes Tran_Date error");
			 strncpy(header_add,send_header.Connection,strlen(send_header.Connection)+1);
					
			}
			else
			{
                puts(send_header.Connection);
      printf("take care!:connection is %s  anything  \n",send_header.Connection);
    
			//	if(MHD_add_response_header(response,"Connection", send_header.Connection)==0)
				if(MHD_add_response_header(response,"Connection", "close")==0)
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
			if(MHD_add_response_header(response,"Content-Type", "text/plain")==0)
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



int get_object_header_no(struct MHD_Response *response,struct sender_head send_header)
{
/*    GET 对象不存在 */

	char *header_add;
	struct stat buf;
	char *temp,*date;
	temp=(char *)malloc(BUFLENGTH*sizeof(char));
	//header_add=(char *)malloc(BUFLENGTH*sizeof(char));
//*******************************NEW CODE******************

	date=(char *)malloc(BUFLENGTH*sizeof(char));
	//header_add=(char *)malloc(BUFLENGTH*sizeof(char));
	header_add = MHD_get_response_header (response,"Date");
	if (header_add != NULL)
	{
		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)
			err_sys("get_object_header_yes Gen_Date error");		
		strncpy(header_add,date,strlen(date)+1);

	}
	else
	{

		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)
			err_sys("get_object_header_no Gen_Date error");
		//printf("%s\n",header_add);
		printf("227 line date: %s\n",date);
	
		//if(MHD_add_response_header(response,"Date", header_add)==0)
		if(MHD_add_response_header(response,"Date", date)==0)
		{
			printf("%s\n",date);
			err_sys("get_object_header_no MHD_add_response_header Date error");
		}

	}
        printf("take care!\n");
		header_add = MHD_get_response_header (response,"Connection");	


			// ltoa(buf.st_size,temp,10);
			if (header_add != NULL)
			{
					
						//err_sys("get_object_header_yes Tran_Date error");
					 strncpy(header_add,send_header.Connection,strlen(send_header.Connection)+1);
					
			}
			else
			{ 
                puts(send_header.Connection);
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




int head_object_header_yes(struct MHD_Response *response,struct sender_head send_header)
{

     /*     GET 对象存在  */

	char *header_add,*temp,*date;
	struct stat buf;
	temp=(char *)malloc(BUFLENGTH*sizeof(char));
	date=(char *)malloc(BUFLENGTH*sizeof(char));
	//header_add=(char *)malloc(BUFLENGTH*sizeof(char));
	header_add = MHD_get_response_header (response,"Date");
	if (header_add != NULL)
	{
		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)
			err_sys("head_object_header_yes Gen_Date error");		
		strncpy(header_add,date,strlen(date)+1);

	}
	else
	{

		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)
			err_sys("head_object_header_yes Gen_Date error");
		//printf("%s\n",header_add);
		printf("62 line date: %s\n",date);
	
		//if(MHD_add_response_header(response,"Date", header_add)==0)
		if(MHD_add_response_header(response,"Date", date)==0)
		{
			printf("%s\n",date);
			err_sys("head_object_header_yes MHD_add_response_header Date error");
		}
	}




	header_add = MHD_get_response_header (response,"Last-Modified");	
	if(stat(send_header.Pathname,&buf)<0)
			err_sys("head_object_header_yes stat error");
	else
	{
			 if(Tran_Date(buf.st_mtime,temp)==0)
			 	err_sys("head_object_header_yes Tran_Date error");
	}

	if (header_add != NULL)
	{
		 strncpy(header_add,temp,strlen(temp)+1);
			
	}
	else
	{
		if(MHD_add_response_header(response,"Last-Modified", temp)==0)
			err_sys("head_object_header_yes MHD_add_response_header Last-Modified error");

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
			err_sys("head_object_header_yes MHD_add_response_header Content-Length error");

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
			err_sys("head_object_header_yes MHD_add_response_header ETag error");

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
					err_sys("head_object_header_yes MHD_add_response_header Connection error");
		
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
				err_sys("head_object_header_yes MHD_add_response_header Content-Length error");
	
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
				err_sys("head_object_header_yes MHD_add_response_header Content-Length error");
			
		}
		
		return 1;
	}




int head_object_header_no(struct MHD_Response *response,struct sender_head send_header)
{
/*    GET 对象不存在 */

	char *header_add;
	struct stat buf;
	char *temp;
	temp=(char *)malloc(BUFLENGTH*sizeof(char));
	//header_add=(char *)malloc(BUFLENGTH*sizeof(char));
	header_add = MHD_get_response_header (response,"Date");
	if (header_add != NULL)
	{
		if(Gen_Date(header_add)==0)
			err_sys("head_object_header_no Gen_Date error");

	}
	else
	{

		if(Gen_Date(header_add)==0)
			err_sys("head_object_header_no Gen_Date error");
		if(MHD_add_response_header(response,"Date", header_add)==0)
			err_sys("head_object_header_no MHD_add_response_header Date error");

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
					err_sys("head_object_header_no MHD_add_response_header Connection error");
		
			}


		if(MHD_add_response_header(response,"Server", "Store B409")==0)
				err_sys("head_object_header_no MHD_add_response_header Content-Length error");
			
	
		

	
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
				err_sys("head_object_header_no MHD_add_response_header x-amz-delete-marker error");
			
		}
		
		return 1;
	}

/* create fresh session */
int post_object_header(struct MHD_Response *response,struct sender_head send_header)
{



     /*     GET 对象存在  */

	char *header_add;
	struct stat buf;
	char *temp;
	temp=(char *)malloc(BUFLENGTH*sizeof(char));
	//header_add=(char *)malloc(BUFLENGTH*sizeof(char));
	header_add = MHD_get_response_header (response,"Date");
	if (header_add != NULL)
	{
		if(Gen_Date(header_add)==0)
			err_sys("post_object_header_yes Gen_Date error");

	}
	else
	{

		if(Gen_Date(header_add)==0)
			err_sys("post_object_header_yes Gen_Date error");
		if(MHD_add_response_header(response,"Date", header_add)==0)
			err_sys("post_object_header_yes MHD_add_response_header Date error");

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
			err_sys("post_object_header_yes MHD_add_response_header ETag error");

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
					err_sys("post_object_header_yes MHD_add_response_header Connection error");
		
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
				err_sys("post_object_header_yes MHD_add_response_header Content-Length error");
	
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
				err_sys("post_object_header_yes MHD_add_response_header Content-Length error");
			
		}
		
		return 1;
	}


int delete_object_header_yes(struct MHD_Response *response,struct sender_head send_header)
{

     char *header_add,*temp,*date;
	struct stat buf;
	temp=(char *)malloc(BUFLENGTH*sizeof(char));
	date=(char *)malloc(BUFLENGTH*sizeof(char));
	//header_add=(char *)malloc(BUFLENGTH*sizeof(char));
	header_add = MHD_get_response_header (response,"Date");
	if (header_add != NULL)
	{
		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)
			err_sys("delete_object_header_yes Gen_Date error");		
		strncpy(header_add,date,strlen(date)+1);

	}
	else
	{

		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)
			err_sys("delete_object_header_yes Gen_Date error");
		//printf("%s\n",header_add);
		printf("62 line date: %s\n",date);
	
		//if(MHD_add_response_header(response,"Date", header_add)==0)
		if(MHD_add_response_header(response,"Date", date)==0)
		{
			printf("%s\n",date);
			err_sys("delete_object_header_yes MHD_add_response_header Date error");
		}
	}


	
	header_add = MHD_get_response_header (response,"Content-Length");	
	/// ltoa(buf.st_size,temp,10);
	if (header_add != NULL)
	{
			
			 	
			 strncpy(header_add,temp,strlen(temp)+1);
			
	}
	else
	{
		if(MHD_add_response_header(response,"Content-Length","0")==0)
			err_sys("delete_object_header_yes MHD_add_response_header Content-Length error");

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
			err_sys("delete_object_header_yes MHD_add_response_header ETag error");

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
					err_sys("delete_object_header_yes MHD_add_response_header Connection error");
		
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
				err_sys("delete_object_header_yes MHD_add_response_header Content-Length error");
			
		}
		
		return 1;
	}




int delete_bucket_header_yes(struct MHD_Response *response,struct sender_head send_header)
{

     char *header_add,*temp,*date;
	struct stat buf;
	temp=(char *)malloc(BUFLENGTH*sizeof(char));
	date=(char *)malloc(BUFLENGTH*sizeof(char));
	//header_add=(char *)malloc(BUFLENGTH*sizeof(char));
	header_add = MHD_get_response_header (response,"Date");
	if (header_add != NULL)
	{

		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)
			err_sys("delete_bucket_header_yes Gen_Date error");		
		strncpy(header_add,date,strlen(date)+1);

	}

	else
	{


		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)

			err_sys("delete_bucket_header_yes Gen_Date error");
		//printf("%s\n",header_add);
		printf("62 line date: %s\n",date);

	
		//if(MHD_add_response_header(response,"Date", header_add)==0)
		if(MHD_add_response_header(response,"Date", date)==0)
		{
			printf("%s\n",date);
			err_sys("delete_bucket_header_yes MHD_add_response_header Date error");
		}
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
					err_sys("delete_bucket_header_yes MHD_add_response_header Connection error");
		
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
				err_sys("delete_bucket_header_yes MHD_add_response_header Content-Length error");
			
		}
		
		return 1;
	}





int head_bucket_header_yes(struct MHD_Response *response,struct sender_head send_header)
{

	
		char *header_add,*temp,*date;
	struct stat buf;
	temp=(char *)malloc(BUFLENGTH*sizeof(char));
	date=(char *)malloc(BUFLENGTH*sizeof(char));
	//header_add=(char *)malloc(BUFLENGTH*sizeof(char));
	header_add = MHD_get_response_header (response,"Date");
	if (header_add != NULL)
	{
		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)
			err_sys("head_bucket_header_yes Gen_Date error");		
		strncpy(header_add,date,strlen(date)+1);

	}
	else
	{

		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)
			err_sys("head_bucket_header_yes Gen_Date error");
		//printf("%s\n",header_add);
		printf("HEAD BUCKET HEADER 919 line in orh.c date: %s\n",date);
	
		//if(MHD_add_response_header(response,"Date", header_add)==0)
		if(MHD_add_response_header(response,"Date", date)==0)
		{
			printf("%s\n",date);
			err_sys("head_bucket_header_yes MHD_add_response_header Date error");
		}
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
					err_sys("head_object_header_yes MHD_add_response_header Connection error");
		
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
				err_sys("head_object_header_yes MHD_add_response_header Server error");
			
		}



		header_add = MHD_get_response_header (response,"x-amz-id-2"); 
				//ltoa(buf.st_size,temp,10);
		if (header_add != NULL)
		{
						
							//err_sys("get_object_header_yes Tran_Date error");
			 strncpy(header_add,"JuKZqmXuiwFeDQxhD7M8KtsKobSzWA1QEjLbTMTagkKdBX2z7Il/jGhDeJ3j6s80",strlen("JuKZqmXuiwFeDQxhD7M8KtsKobSzWA1QEjLbTMTagkKdBX2z7Il/jGhDeJ3j6s80")+1);
						
		}
		else
		{
			if(MHD_add_response_header(response,"x-amz-id-2", "JuKZqmXuiwFeDQxhD7M8KtsKobSzWA1QEjLbTMTagkKdBX2z7Il/jGhDeJ3j6s80")==0)
				err_sys("head_object_header_yes MHD_add_response_header x-amz-id-2 error");
			
		}



		header_add = MHD_get_response_header (response,"x-amz-request-id"); 
				//ltoa(buf.st_size,temp,10);
		if (header_add != NULL)
		{
						
							//err_sys("get_object_header_yes Tran_Date error");
			 strncpy(header_add,"32FE2CEB32F5EE25",strlen("32FE2CEB32F5EE25")+1);
						
		}
		else
		{
			if(MHD_add_response_header(response,"x-amz-request-id", "32FE2CEB32F5EE25")==0)
				err_sys("head_object_header_yes MHD_add_response_header x-amz-request-id error");
			
		}



		
		return 1;
	}




int head_bucket_header_no(struct MHD_Response *response,struct sender_head send_header)
{

	char *header_add;
	struct stat buf;
	char *temp,*date;
	temp=(char *)malloc(BUFLENGTH*sizeof(char));
	//header_add=(char *)malloc(BUFLENGTH*sizeof(char));
//*******************************NEW CODE******************

	date=(char *)malloc(BUFLENGTH*sizeof(char));
	//header_add=(char *)malloc(BUFLENGTH*sizeof(char));
	header_add = MHD_get_response_header (response,"Date");
	if (header_add != NULL)
	{
		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)
			err_sys("get_object_header_yes Gen_Date error");		
		strncpy(header_add,date,strlen(date)+1);

	}
	else
	{

		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)
			err_sys("head_bucket_header_no Gen_Date error");
		//printf("%s\n",header_add);
		printf("227 line date: %s\n",date);
	
		//if(MHD_add_response_header(response,"Date", header_add)==0)
		if(MHD_add_response_header(response,"Date", date)==0)
		{
			printf("%s\n",date);
			err_sys("head_bucket_header_no MHD_add_response_header Date error");
		}

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
					err_sys("head_bucket_header_no MHD_add_response_header Connection error");
		
			}

		if(MHD_add_response_header(response,"Server", "Store B409")==0)
				err_sys("head_bucket_header_no MHD_add_response_header Content-Length error");
			
	
		

	
		header_add = MHD_get_response_header (response,"Server"); 
				//ltoa(buf.st_size,temp,10);
		if (header_add != NULL)
		{
						
							//err_sys("head_bucket_header_no Tran_Date error");
			 strncpy(header_add,"Store B409",strlen("Store B409")+1);
						
		}
		else
		{
			if(MHD_add_response_header(response,"x-amz-delete-marker", "TRUE")==0)
				err_sys("head_bucket_header_no MHD_add_response_header x-amz-delete-marker error");
			
		}
		
		return 1;









}

int put_bucket_header_yes(struct MHD_Response *response,struct sender_head send_header)
{

	
		char *header_add,*temp,*date;
	struct stat buf;
	temp=(char *)malloc(BUFLENGTH*sizeof(char));

	date=(char *)malloc(BUFLENGTH*sizeof(char));
	
	header_add = MHD_get_response_header (response,"Date");

	if (header_add != NULL)
	{
		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)
			err_sys("post_bucket_header_yes Gen_Date error");		
		strncpy(header_add,date,strlen(date)+1);

	}
	else
	{


		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)

			err_sys("post_bucket_header_yes Gen_Date error");
		//printf("%s\n",header_add);
		printf("HEAD BUCKET HEADER 919 line in orh.c date: %s\n",date);
	
		//if(MHD_add_response_header(response,"Date", header_add)==0)

		if(MHD_add_response_header(response,"Date", date)==0)
		{
			printf("%s\n",date);

			err_sys("post_bucket_header_yes MHD_add_response_header Date error");
		}

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
					err_sys("post_bucket_header_yes MHD_add_response_header Connection error");

		

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
				err_sys("post_bucket_header_yes MHD_add_response_header Server error");
			
		}



		header_add = MHD_get_response_header (response,"x-amz-id-2"); 
				//ltoa(buf.st_size,temp,10);
		if (header_add != NULL)
		{
						
							//err_sys("get_object_header_yes Tran_Date error");
			 strncpy(header_add,"JuKZqmXuiwFeDQxhD7M8KtsKobSzWA1QEjLbTMTagkKdBX2z7Il/jGhDeJ3j6s80",strlen("JuKZqmXuiwFeDQxhD7M8KtsKobSzWA1QEjLbTMTagkKdBX2z7Il/jGhDeJ3j6s80")+1);
						
		}
		else
		{
			if(MHD_add_response_header(response,"x-amz-id-2", "JuKZqmXuiwFeDQxhD7M8KtsKobSzWA1QEjLbTMTagkKdBX2z7Il/jGhDeJ3j6s80")==0)
				err_sys("post_bucket_header_yes MHD_add_response_header x-amz-id-2 error");
			
		}



		header_add = MHD_get_response_header (response,"x-amz-request-id"); 
				//ltoa(buf.st_size,temp,10);
		if (header_add != NULL)
		{
						
							//err_sys("get_object_header_yes Tran_Date error");
			 strncpy(header_add,"32FE2CEB32F5EE25",strlen("32FE2CEB32F5EE25")+1);
						
		}
		else
		{
			if(MHD_add_response_header(response,"x-amz-request-id", "32FE2CEB32F5EE25")==0)
				err_sys("post_bucket_header_yes MHD_add_response_header x-amz-request-id error");
			
		}	
		return 1;
	}






int get_bucket_header_yes(struct MHD_Response *response,struct sender_head send_header)
{

	char *header_add,*temp,*date;
	struct stat buf;
	temp=(char *)malloc(BUFLENGTH*sizeof(char));
	date=(char *)malloc(BUFLENGTH*sizeof(char));
	//header_add=(char *)malloc(BUFLENGTH*sizeof(char));
	header_add = MHD_get_response_header (response,"Date");
	if (header_add != NULL)
	{
		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)
			err_sys("get_bucket_header_yes Gen_Date error");		
		strncpy(header_add,date,strlen(date)+1);

	}
	else
	{

		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)
			err_sys("get_bucket_header_yes Gen_Date error");
		//printf("%s\n",header_add);
		printf("972 line date: %s\n",date);
	
		//if(MHD_add_response_header(response,"Date", header_add)==0)
		if(MHD_add_response_header(response,"Date", date)==0)
		{
			printf("%s\n",date);
			err_sys("get_bucket_header_yes MHD_add_response_header Date error");
		}
	}
	
	header_add = MHD_get_response_header (response,"Date");
	printf("get response header date : %s \n",header_add);






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
					err_sys("get_bucket_header_yes MHD_add_response_header Connection error");
		
			}


	
    	header_add = MHD_get_response_header (response,"Content-Type");	
		//ltoa(buf.st_size,temp,10);
		if (header_add != NULL)
		{
				
					//err_sys("get_object_header_yes Tran_Date error");
				 strncpy(header_add,"application/xml",strlen("application/xml")+1);
				
		}
		else
		{
			if(MHD_add_response_header(response,"application/xml", "application/xml")==0)
				err_sys("get_bucket_header_yes MHD_add_response_header Content-Length error");
	
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
		free(temp);
		free(date);              
		return 1;                        
	}




int get_bucket_header_no(struct MHD_Response *response,struct sender_head send_header)
{

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
}




int post_bucket_header_yes(struct MHD_Response *response,struct sender_head send_header)
{

	
		char *header_add,*temp,*date;
	struct stat buf;
	temp=(char *)malloc(BUFLENGTH*sizeof(char));
	date=(char *)malloc(BUFLENGTH*sizeof(char));
	//header_add=(char *)malloc(BUFLENGTH*sizeof(char));
	header_add = MHD_get_response_header (response,"Date");
	if (header_add != NULL)
	{
		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)
			err_sys("post_bucket_header_yes Gen_Date error");		
		strncpy(header_add,date,strlen(date)+1);

	}
	else
	{

		//if(Gen_Date(header_add)==0)
		if(Gen_Date(date)==0)
			err_sys("post_bucket_header_yes Gen_Date error");
		//printf("%s\n",header_add);
		printf("HEAD BUCKET HEADER 919 line in orh.c date: %s\n",date);
	
		//if(MHD_add_response_header(response,"Date", header_add)==0)
		if(MHD_add_response_header(response,"Date", date)==0)
		{
			printf("%s\n",date);
			err_sys("post_bucket_header_yes MHD_add_response_header Date error");
		}
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
					err_sys("post_bucket_header_yes MHD_add_response_header Connection error");
		
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
				err_sys("post_bucket_header_yes MHD_add_response_header Server error");
			
		}



		header_add = MHD_get_response_header (response,"x-amz-id-2"); 
				//ltoa(buf.st_size,temp,10);
		if (header_add != NULL)
		{
						
							//err_sys("get_object_header_yes Tran_Date error");
			 strncpy(header_add,"JuKZqmXuiwFeDQxhD7M8KtsKobSzWA1QEjLbTMTagkKdBX2z7Il/jGhDeJ3j6s80",strlen("JuKZqmXuiwFeDQxhD7M8KtsKobSzWA1QEjLbTMTagkKdBX2z7Il/jGhDeJ3j6s80")+1);
						
		}
		else
		{
			if(MHD_add_response_header(response,"x-amz-id-2", "JuKZqmXuiwFeDQxhD7M8KtsKobSzWA1QEjLbTMTagkKdBX2z7Il/jGhDeJ3j6s80")==0)
				err_sys("post_bucket_header_yes MHD_add_response_header x-amz-id-2 error");
			
		}



		header_add = MHD_get_response_header (response,"x-amz-request-id"); 
				//ltoa(buf.st_size,temp,10);
		if (header_add != NULL)
		{
						
							//err_sys("get_object_header_yes Tran_Date error");
			 strncpy(header_add,"32FE2CEB32F5EE25",strlen("32FE2CEB32F5EE25")+1);
						
		}
		else
		{
			if(MHD_add_response_header(response,"x-amz-request-id", "32FE2CEB32F5EE25")==0)
				err_sys("post_bucket_header_yes MHD_add_response_header x-amz-request-id error");
			
		}



		
		return 1;
	}

int put_send_page (struct MHD_Connection *connection, const char *page,int status_code)
	{
		int ret;
		struct MHD_Response *response;
		sender_head send_header;
		response =
		MHD_create_response_from_buffer (strlen (page), (void *) page,MHD_RESPMEM_PERSISTENT);
		if (!response)
		{
			perror("send_page MHD_create_response_from_buffer");
			return MHD_NO;
		}
		post_object_header(response,send_header);
		MHD_queue_response (connection, status_code, response);
		ret=MHD_queue_response (connection, MHD_HTTP_CONTINUE, response);
		MHD_destroy_response (response);
		return ret;
	}


int iterate_put (void *coninfo_cls, enum MHD_ValueKind kind, const char *key,const char *filename, const char *content_type,const char *transfer_encoding, const char *data, uint64_t off,size_t size)
	{
		struct connection_info_struct *con_info = coninfo_cls;
		FILE *fp;
		con_info->answerstring = servererrorpage;
		con_info->answercode = MHD_HTTP_INTERNAL_SERVER_ERROR;
		if (0 != strcmp (key, "file"))
			return MHD_NO;
        char pathname[256];
        get_cache_path(filename,pathname);
       // time_t arrive_time;
        //time(&arrive_time);
        //IO_Type io_type=WRITE;

		if (!con_info->fp)
		{

			if (NULL != (fp = fopen (filename, "rb")))
			{
				fclose (fp);
				con_info->answerstring = fileexistspage;
				con_info->answercode = MHD_HTTP_FORBIDDEN;
				return MHD_NO;
			}
			/*************add by Jin:io_queue***************/
            //queue_in_wait(pathname,io_type,arrive_time);
            /***********************************************/
            con_info->fp = fopen (pathname, "wb");
			if (!con_info->fp)
			return MHD_NO;
		}
		if (size > 0)
		{
			if (!fwrite (data, size, sizeof (char), con_info->fp))
			return MHD_NO;
		}
		con_info->answerstring = completepage;
		con_info->answercode = MHD_HTTP_OK;
        
        //u64 offset1=0;char *data1="never mind!";size_t size1=11;
        //write_queue_out(pathname,io_type,arrive_time,offset1,data1,size1);//should change it
		return MHD_YES;
	}



void put_request_completed (void *cls, struct MHD_Connection *connection,void **con_cls, enum MHD_RequestTerminationCode toe)
	{
		struct connection_info_struct *con_info = *con_cls;
		if (NULL == con_info)
		return;
		if (con_info->connectiontype == POST)
		{
			if (NULL != con_info->postprocessor)
			{
				MHD_destroy_post_processor (con_info->postprocessor);
				//nr_of_uploading_clients--;
			}
			if (con_info->fp)
				fclose (con_info->fp);
		}
		free (con_info);
		*con_cls = NULL;
	}


int post_send_page (struct MHD_Connection *connection, const char *page,int status_code)
	{
		int ret;
		struct MHD_Response *response;
		sender_head send_header;
		response =
		MHD_create_response_from_buffer (strlen (page), (void *) page,MHD_RESPMEM_PERSISTENT);
		if (!response)
		{
			perror("post_send_page MHD_create_response_from_buffer");
			return MHD_NO;
		}
		post_object_header(response,send_header);
		MHD_queue_response (connection, status_code, response);
		ret=MHD_queue_response (connection, MHD_HTTP_CONTINUE, response);
		MHD_destroy_response (response);
		return ret;
	}
//**********************************************
/*
int iterate_post(void *coninfo_cls, enum MHD_ValueKind kind, const char *key,const char *filename, const char *content_type,const char *transfer_encoding, const char *data, uint64_t off,size_t size)
	{
		struct connection_info_struct *con_info = coninfo_cls;
		FILE *fp;
		con_info->answerstring = servererrorpage;
		con_info->answercode = MHD_HTTP_INTERNAL_SERVER_ERROR;
		if (0 != strcmp (key, "file"))
			return MHD_NO;
		if (!con_info->fp)
		{

			if (NULL != (fp = fopen (filename, "rb")))
			{
				fclose (fp);
				con_info->answerstring = fileexistspage;
				con_info->answercode = MHD_HTTP_FORBIDDEN;
				return MHD_NO;
			}
			con_info->fp = fopen (filename, "ab");
			if (!con_info->fp)
			return MHD_NO;
		}
		if (size > 0)
		{
			if (!fwrite (data, size, sizeof (char), con_info->fp))
			return MHD_NO;
		}
		con_info->answerstring = completepage;
		con_info->answercode = MHD_HTTP_OK;
		return MHD_YES;
	}

*/
//*********************************************************
void post_request_completed (void *cls, struct MHD_Connection *connection,void **con_cls, enum MHD_RequestTerminationCode toe)
	{
		struct connection_info_struct *con_info = *con_cls;
		if (NULL == con_info)
		return;
		if (con_info->connectiontype == POST)
		{
			if (NULL != con_info->postprocessor)
			{
				MHD_destroy_post_processor (con_info->postprocessor);
				//nr_of_uploading_clients--;
			}
			if (con_info->fp)
				fclose (con_info->fp);
		}
		free (con_info);
		*con_cls = NULL;
	}
//************************************************************************
//************************************************************************


 int
send_page (struct MHD_Connection *connection, const char *page,
           int status_code,const char *pathname,const char *client_ip)
{
  int ret;
  struct MHD_Response *response;

  response =
    MHD_create_response_from_buffer (strlen (page), (void *) page,
				     MHD_RESPMEM_MUST_COPY);
  if (!response)
    return MHD_NO;
  MHD_add_response_header (response, MHD_HTTP_HEADER_CONTENT_TYPE, "text/html");
  ret = MHD_queue_response (connection, status_code, response);
  MHD_destroy_response (response);
  --nr_of_uploading_clients;//decrease the con-clients
  printf("after:%d\n",nr_of_uploading_clients);
  char from_path[128];
  char to_path[128];
  sprintf(from_path,"/mnt/supercache/%s.%s",pathname,client_ip);
  sprintf(to_path,"/mnt/supercache/%s",pathname);
  char command[256];
  sprintf(command,"mv %s %s",from_path,to_path);
  //printf("+++++++++++++++++++++++++++++++++++++++++++++%s\n",command);
  system(command);
  //printf("+++++++++++++++++++++++++++++++++++++++++%s\n",from_path);
  //printf("+++++++++++++++++++++++++++++++++++++++++%s\n",to_path);
  return ret;
}


int
iterate_post (void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
              const char *filename, const char *content_type,
              const char *transfer_encoding, const char *data, uint64_t off,
              size_t size)
{
  struct connection_info_struct *con_info = coninfo_cls;
  FILE *fp;

//	printf("now in iterate_post!\n");
//	printf("the data is : %s\n",data);
	//printf("the data size is %d \n",size);

//********************************20130515***************************


   // printf("IP of client is : %s\n",con_info->client_ip);
    //return MHD_NO;

//********************************20130515*****************************
    
  con_info->answerstring = servererrorpage;
  con_info->answercode = MHD_HTTP_INTERNAL_SERVER_ERROR;
  //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  //**************************************** 20130602****************************
//    *************************************20130603*************************
//    printf("filename  is  &&&  %s \n",filename);
  if (0 != strcmp (key, "file"))
{
	printf(" key is not file! quit out! \n");
    return MHD_NO;
}

    char temp_filename[256];
    sprintf(temp_filename,"%s.%s",filename,con_info->client_ip);
    //printf("@#@#@#@#@#@#@@##@#@#@#@#@#@#@#@@#@##%s\n",temp_filename);
    char pathname[256];
    get_cache_path(temp_filename,pathname);
    
    //printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$%d\n",getpid());
    //time_t arrive_time;
    //time(&arrive_time);
    //IO_Type io_type=WRITE;
  if (!con_info->fp)
    {
      if (NULL != (fp = fopen (filename, "rb")))
        {
          fclose (fp);
          con_info->answerstring = fileexistspage;
          con_info->answercode = MHD_HTTP_FORBIDDEN;
          return MHD_NO;
        }

     // con_info->fp = fopen (filename, "ab");
   /**************************add by Jin************/
    
   // queue_in_wait(pathname,io_type,arrive_time);
    /**********************************************/
      con_info->fp = fopen (pathname, "wb");
      if (!con_info->fp)
        return MHD_NO;
    }

  if (size > 0)
    {
      if (!fwrite (data, size, sizeof (char), con_info->fp))
        return MHD_NO;
    }

  con_info->answerstring = completepage;
  con_info->answercode = MHD_HTTP_OK;
  //u64 offset1=0;char *data1="never mind!";size_t size1=11;
  //write_queue_out(pathname,io_type,arrive_time,offset1,data1,size1);//should change it

  return MHD_YES;
}


/*void
request_completed (void *cls, struct MHD_Connection *connection,
                   void **con_cls, enum MHD_RequestTerminationCode toe)
{
  struct connection_info_struct *con_info = *con_cls;

  if (NULL == con_info)
    return;

  if (con_info->connectiontype == POST)
    {
      if (NULL != con_info->postprocessor)
        {
          MHD_destroy_post_processor (con_info->postprocessor);
          nr_of_uploading_clients--;
        }

      if (con_info->fp)
        fclose (con_info->fp);
    }

  free (con_info);
  *con_cls = NULL;
}*/


int Gen_Date(char *date)
{
        struct tm *tmptr;
        time_t *calptr=(time_t *)malloc(BUFLENGTH*sizeof(time_t));
        if(time(calptr)<0)
	{
		err_sys("time error!\n");
		return 0;
        }
        else
        {
                printf("%.24s\r\n",ctime(calptr));
                tmptr=gmtime(calptr);
		  // date=(char*)malloc(128*sizeof(char));
		if(date==NULL)
		{
			printf("1297 line date is NULL!quit\n");
			return 0;
		}  
		printf("hello date!\n");

                if(strftime(date,BUFLENGTH,"%a,%e %b %Y %X %Z",tmptr)==0)
                {      
			puts("Hello!\n");               	
			puts(date);
                        err_sys("strftime error\n");
            		return 0;
                }
		printf(" date is %s \n",date);             
           //return buf;
        }
	free(calptr);
	return 1;
       
}






int err_sys(const char *info)                                                        
{                                                                                 
        fprintf(stderr, "%s:%s\n", info, strerror(errno));
        exit(EXIT_FAILURE);
}

int Tran_Date(time_t time,char *date)
{
	struct tm *tmptr;	
	tmptr=gmtime(&time);
       	if( strftime(date,BUFLENGTH,"%a,%e %b %Y %X %Z",tmptr)==0)
       	{
                	//puts(date);
                        err_sys("strftime error\n");
            		return 0;
          }
          else return 1;              //return buf;
 }








