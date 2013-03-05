#include"md_mutex_glob.h"
#include"md_mutex_server.h"
#include"md_mutex_proto.h"
#include<pthread.h>
static pthread_mutex_t glob_mutex = PTHREAD_MUTEX_INITIALIZER;
static z_mutex mutex_table[MUTEX_NUM];/* first initialized in main */
int mutex_table_init()
{
	z_mutex * my_mutex; 
	bzero(mutex_table,MUTEX_NUM*sizeof(z_mutex));
	for(my_mutex = &mutex_table[0];my_mutex < &mutex_table[MUTEX_NUM];my_mutex++)
	{
		pthread_mutex_init(&my_mutex->p_mutex,NULL);
		my_mutex->count = 0;
	//	my_mutex->flag = MTX_FREE;
	}
	return 0;
}
int show_mutex_table()
{
	z_mutex * my_mutex;
	int i = 0;
	int r = 0;
	if(pthread_mutex_lock(&glob_mutex) != 0)
	{
		perror("glob_mutex thread_mutex_lock");
		r = 1;
	}
	printf("------------------------mutex_table--------------------\n");
	for(my_mutex = &mutex_table[0];my_mutex < &mutex_table[MUTEX_NUM];my_mutex++)
	{
		printf("%3d %10s %3d   %s\n",i++,((my_mutex->count == 0) ? MTX_FREE_S:MTX_TAKEN_S),my_mutex->count,my_mutex->file_name);
	}
	printf("-------------------------------------------------------\n");
	if(pthread_mutex_unlock(&glob_mutex) != 0)
	{
		perror("glob_mutex thread_mutex_unlock");
		r = 2;
	}
	return r;
}
z_mutex * get_z_mutex(char * file_name,char op_type)
{
	int i = 0;
	z_mutex * my_mutex;
	z_mutex * first_free_mutex = (z_mutex *)0;
	if(pthread_mutex_lock(&glob_mutex) != 0)
	{
		perror("glob_mutex thread_mutex_lock");
	}
	printf("Now getz_mutex for file -- %s\n",file_name);
	if(op_type == MD_LOCK)
	{
		printf("op_type -- MD_LOCK\n");
	}
	else if(op_type == MD_TRY_LOCK)
	{
		printf("op_type -- MD_TRY_LOCK\n");
	}
	else if(op_type == MD_UNLOCK)
	{
		printf("op_type -- MD_UNLOCK\n");
	}
	printf("--------------now traverse mutex_table------------------\n");
	for(i=0,my_mutex = &mutex_table[0];my_mutex < &mutex_table[MUTEX_NUM];my_mutex++,i++)
	{
		printf("%3d %10s %3d   %s\n",i,((my_mutex->count == 0) ? MTX_FREE_S:MTX_TAKEN_S),my_mutex->count,my_mutex->file_name);
		if(my_mutex->count == 0)
		{
			/* mutex not taken */
			printf(" -- mutex %d is free\n",i);
			if(first_free_mutex == (z_mutex *)0)
			{
				printf("the first_free_mutex is NULL , assign first_free_mutex with mutex %d\n",i);
				first_free_mutex = my_mutex;
				continue;
			}
		}
		else if(my_mutex->count > 0)
		{
			printf("-- mutex %d is not free,taken by file -- %s\n",i,my_mutex->file_name);
			if(strcmp(my_mutex->file_name,file_name) == 0)
			{
				printf(" same file with the req,break--\n");
				break;
			}
			printf(" but not the same file with the req, continue\n");
			continue;
		}
		else{/* error */ exit(EXIT_FAILURE);}
	}
	printf("out of FOR LOOP\n");
	if(op_type == MD_LOCK || op_type == MD_TRY_LOCK)
	{
		printf("lock operation\n");
		if(my_mutex == &mutex_table[MUTEX_NUM])
		{
			printf("No match for the same file,use the first free mutex\n");
			if(first_free_mutex == (z_mutex *)0)
			{
				/* no free mutex */
				printf("but no free mutex...\n");
				my_mutex = first_free_mutex;
			}
			else
			{
				my_mutex = first_free_mutex;
				bzero(my_mutex->file_name,MAX_PATH);
				strcpy(my_mutex->file_name,file_name);
				/* mark this mutex taken */
			//	my_mutex->flag = MTX_TAKEN;
				my_mutex->count = 1;
			}
		}
		else
		{
			my_mutex->count++;
		}
	}
	else if(op_type == MD_UNLOCK)
	{
		if(my_mutex == &mutex_table[MUTEX_NUM])
		{
			printf("but no match mutex for the req\n");
			/* error,could not find the mutex to unlock */
			my_mutex = (z_mutex *)0;
		}
	}
	if(pthread_mutex_unlock(&glob_mutex) != 0)
	{
		perror("glob_mutex pthread_mutex_unlock");
	}
	return my_mutex;
}
int put_z_mutex(z_mutex * my_mutex)
{
	int r = 0;
	if(pthread_mutex_lock(&glob_mutex) != 0)
	{
		perror("glob_mutex pthread_mutex_lock");
		r = 1;
	}
	my_mutex->count--;
	if(pthread_mutex_unlock(&glob_mutex) != 0)
	{
		perror("glob_mutex pthread_mutex_unlock");
		r = 2;
	}
	return r;
}
/*
int up_count(z_mutex * my_mutex)
{
	int r = 0;
	if(pthread_mutex_lock(&glob_mutex) != 0)
	{
		perror("glob_mutex pthread_mutex_lock");
		r = 1;
	}
	my_mutex->count++;
	if(pthread_mutex_unlock(&glob_mutex) != 0)
	{
		perror("glob_mutex pthread_mutex_unlock");
		r = 2;
	}
	return r;
}
int down_count(z_mutex * my_mutex)
{
	int r = 0;
	if(pthread_mutex_lock(&glob_mutex) != 0)
	{
		perror("glob_mutex pthread_mutex_lock");
		r = 1;
	}
	if(--my_mutex->count == 0)
	{
		my_mutex->flag = MTX_FREE;
	}
	if(pthread_mutex_unlock(&glob_mutex) != 0)
	{
		perror("glob_mutex pthread_mutex_unlock");
		r = 2;
	}
	return r;
}
*/
void * thread_func(void * arg)
{
	thread_arg * my_arg = (thread_arg *)arg;
	z_mutex * my_mutex;
	md_mutex_res_packet * res_pkt = (md_mutex_res_packet*)malloc(sizeof(md_mutex_res_packet));
	res_pkt->ok_or_not = OK;
	if((my_mutex = get_z_mutex(my_arg->file_name,my_arg->op_type)) == (z_mutex *)0)
	{
		/* fail to get a valid mutex from mutex_table */
		res_pkt->why_fail = ALLOC_MUTEX_FAIL;
		res_pkt->ok_or_not = NOT_OK;
	}
	else
	{
		switch(my_arg->op_type)
		{
			case MD_LOCK:
				printf("LOCK operation,after get_z_mutex:\n");
				show_mutex_table();
				if(pthread_mutex_lock(&my_mutex->p_mutex) != 0)
				{
					printf("Lock fail! put z_mutex back\n");
					put_z_mutex(my_mutex);
					res_pkt->why_fail = LOCK_FAIL;
					res_pkt->ok_or_not = NOT_OK;
				}
				else{printf("Lock success!\n");}
				break;
			case MD_TRY_LOCK:
				printf("TRY_LOCK operation,after get_z_mutex:\n");
				show_mutex_table();
				if(pthread_mutex_trylock(&my_mutex->p_mutex) != 0)
				{
					printf("Try_Lock fail! put z_mutex back\n");
					put_z_mutex(my_mutex);
					res_pkt->why_fail = TRY_LOCK_FAIL;
					res_pkt->ok_or_not = NOT_OK;
				}
				else{printf("Try_Lock success!\n");}
				break;
			case MD_UNLOCK:
				if(pthread_mutex_unlock(&my_mutex->p_mutex) == 0)
				{
					printf("Unlock success! put z_mutex back\n");
					put_z_mutex(my_mutex);
				}
				else
				{
					printf("Unlock fail!\n");
					res_pkt->why_fail = UNLOCK_FAIL;
					res_pkt->ok_or_not = NOT_OK;
				}
				break;
			default:
				/* unrecognized op_type */
				res_pkt->why_fail = UNRECOGNIZED_OP_TYPE;
				res_pkt->ok_or_not = NOT_OK;
				break;
		}
		show_mutex_table();
	}
	if(write(my_arg->client_sock_fd,res_pkt,sizeof(md_mutex_res_packet)) == -1)
	{
		perror("write");
	}
	close(my_arg->client_sock_fd);
	free((thread_arg *)arg);
	free(res_pkt);
	pthread_exit((void *)0);
}
int main()
{
	pthread_t thread_id;
	thread_arg * th_arg;
	md_mutex_req_packet req_pkt;
	char clnt_ip[INET_ADDRSTRLEN];
	int clnt_port;
	int listen_fd,connect_fd,addr_len,clnt_addr_len,n;
	struct sockaddr_in addr,clnt_addr;
	addr_len = sizeof(addr);
	bzero(&addr,addr_len);
	addr.sin_family = AF_INET;
	inet_pton(AF_INET,MD_MUTEX_SERV_IP,(void*)&addr.sin_addr);
	addr.sin_port = htons(MD_MUTEX_SERV_PORT);
	listen_fd = socket(AF_INET,SOCK_STREAM,0);
	if(bind(listen_fd,(struct sockaddr*)&addr,addr_len) == -1)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}
	mutex_table_init();
	show_mutex_table();
	listen(listen_fd,MD_MUTEX_SERV_MAX_LISTEN_Q);
	for(;;)
	{
		printf("md_concurrent_access_control_server listening...\n");
		bzero(clnt_ip,INET_ADDRSTRLEN);
		bzero(&req_pkt,sizeof(req_pkt));
		connect_fd = accept(listen_fd,(struct sockaddr*)&clnt_addr,&clnt_addr_len);
		inet_ntop(AF_INET,(void *)&clnt_addr.sin_addr,clnt_ip,INET_ADDRSTRLEN);
		clnt_port = ntohs(clnt_addr.sin_port);
		if((n = read(connect_fd,&req_pkt,sizeof(req_pkt))) == -1)
		{
			perror("read");
			exit(EXIT_FAILURE);
		}
		printf("%d bytes read from client : ip - %s ^^ port - %d\n",n,clnt_ip,clnt_port);
		th_arg = (thread_arg*)malloc(sizeof(thread_arg));
		if(th_arg == NULL)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		bzero(th_arg,sizeof(thread_arg));
		th_arg->client_sock_fd = connect_fd;
		strcpy(th_arg->file_name,req_pkt.file_name);
		th_arg->op_type = req_pkt.op_type;
		/* start lock/try_lock thread */
		if(pthread_create(&thread_id,NULL,thread_func,(void*)th_arg) != 0)
		{
			perror("lock_thread_create");
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}
