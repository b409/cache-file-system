#include"md_mutex_glob.h"
#include"md_mutex_server.h"
#include"md_mutex_proto.h"
#include<pthread.h>
static pthread_mutex_t glob_mutex = PTHREAD_MUTEX_INITIALIZER;
static zz_mutex * mutex_hash[NHASH];
int mutex_hash_init()
{
	int i;
	for(i = 0;i < NHASH;i++)
	{
		mutex_hash[i] = (zz_mutex*)0;
	}
	return 0;
}
int show_mutex_hash()
{
	int r = 0;
	int i,hash_len;
//	float avg_len;
	zz_mutex * my_mutex;
	if(pthread_mutex_lock(&glob_mutex) != 0)
	{
		r = 1;
		perror("glob_mutex thread_mutex_lock");
	}
	for(i = 0;i < NHASH;i++)
	{
		if((my_mutex = mutex_hash[i]) == (zz_mutex*)0){continue;}
		printf("--------Hash%3d ---------\n",i);
		hash_len = 0;
		while(my_mutex)
		{
			hash_len++;
			printf("  %3d   %s\n",my_mutex->count,my_mutex->file_name);
			my_mutex = my_mutex->next;
		}
		printf("--------Hash%3d over---len%3d------\n",i,hash_len);
	}
	if(pthread_mutex_unlock(&glob_mutex) != 0)
	{
		r = 2;
		perror("glob_mutex thread_mutex_unlock");
	}
	return r;
}
zz_mutex * alloc_zz_mutex()
{
	zz_mutex * mtx = (zz_mutex*)malloc(sizeof(zz_mutex));
	if(pthread_mutex_init(&mtx->p_mutex,NULL) != 0)
	{
		perror("pthread_mutex initialize fail");
		free(mtx);
		mtx = (zz_mutex*)0;
	}
	return mtx;
}
int free_zz_mutex(zz_mutex * mtx)
{
	int r = 0;
	if(pthread_mutex_destroy(&mtx->p_mutex) != 0)
	{
		perror("pthread_mutex_destroy");
		r = 1;
	}
	free(mtx);
	return r;
}
zz_mutex * get_zz_mutex(char * file_name,char op_type)
{
	zz_mutex * my_mutex;
	zz_mutex * nmtx;
	unsigned int hash = hash_func(file_name);
	my_mutex = mutex_hash[hash];
	if(pthread_mutex_lock(&glob_mutex) != 0)
	{
		perror("glob_mutex thread_mutex_lock");
	}
	printf("now traverse hash_chain\n");
	while(my_mutex != (zz_mutex*)0)
	{
		printf("count -- %d file_name -- %s\n",my_mutex->count,my_mutex->file_name);
		if(strcmp(file_name,my_mutex->file_name) == 0){break;}
		my_mutex = my_mutex->next;
		continue;
	}
	printf("%s -- hash --%d\n",file_name,hash);
	if(op_type == MD_LOCK || op_type == MD_TRY_LOCK)
	{
		if(my_mutex == (zz_mutex*)0)
		{
			printf("no match mutex for req,now allocate a new mutex \n");
			if((nmtx = alloc_zz_mutex()) == (zz_mutex*)0)
			{
				printf("alloc_zz_mutex fail\n");
			}
			else
			{
				printf("alloc_mutex success!\n");
				my_mutex = nmtx;
				strcpy(nmtx->file_name,file_name);
				nmtx->count = 1;
				nmtx->next = mutex_hash[hash];
				nmtx->pre = (zz_mutex*)0;
				if(mutex_hash[hash] != (zz_mutex*)0)
				{
					printf("mutex_hash[%d] is null\n",hash);
					mutex_hash[hash]->pre = nmtx;
				}
				mutex_hash[hash] = nmtx;
			}
		}
		else
		{
			printf("find a match muutex for lock || try_lock operation \n");
			my_mutex->count++;
		}
	}
	else if(op_type == MD_UNLOCK)
	{
		if(my_mutex == (zz_mutex*)0)
		{
			/* error */
			printf("no mutex to unlock!\n");
		}
		else{printf("find match mutex for unlock operation\n");}
	}
	if(pthread_mutex_unlock(&glob_mutex) != 0)
	{
		perror("glob_mutex thread_mutex_unlock");
	}
	return my_mutex;
}
int put_zz_mutex(zz_mutex * my_mutex)
{
	int r = 0;
	int hash;
	if(pthread_mutex_lock(&glob_mutex) != 0)
	{
		r = 1;
		perror("glob_mutex thread_mutex_lock");
	}
	if(--my_mutex->count == 0)
	{
		if(my_mutex->next != (zz_mutex*)0)
		{
			my_mutex->next->pre = my_mutex->pre;
		}
		if(my_mutex->pre != (zz_mutex*)0)
		{
			my_mutex->pre->next = my_mutex->next;
		}
		else
		{
			/*my_mutex is head*/
			hash = hash_func(my_mutex->file_name);
			mutex_hash[hash] = my_mutex->next;
		}
		free_zz_mutex(my_mutex);
	}
	if(pthread_mutex_unlock(&glob_mutex) != 0)
	{
		r = 2;
		perror("glob_mutex thread_mutex_unlock");
	}
	return r;
}
void * thread_func(void * arg)
{
	thread_arg * my_arg = (thread_arg *)arg;
	zz_mutex * my_mutex;
	md_mutex_res_packet * res_pkt = (md_mutex_res_packet*)malloc(sizeof(md_mutex_res_packet));
	res_pkt->ok_or_not = OK;
	if((my_mutex = get_zz_mutex(my_arg->file_name,my_arg->op_type)) == (zz_mutex *)0)
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
				printf("LOCK operation,after get_zz_mutex:\n");
				show_mutex_hash();
				if(pthread_mutex_lock(&my_mutex->p_mutex) != 0)
				{
					printf("Lock fail! put zz_mutex back\n");
					put_zz_mutex(my_mutex);
					res_pkt->why_fail = LOCK_FAIL;
					res_pkt->ok_or_not = NOT_OK;
				}
				else{printf("Lock success!\n");}
				break;
			case MD_TRY_LOCK:
				printf("TRY_LOCK operation,after get_zz_mutex:\n");
				show_mutex_hash();
				if(pthread_mutex_trylock(&my_mutex->p_mutex) != 0)
				{
					printf("Try_Lock fail! put zz_mutex back\n");
					put_zz_mutex(my_mutex);
					res_pkt->why_fail = TRY_LOCK_FAIL;
					res_pkt->ok_or_not = NOT_OK;
				}
				else{printf("Try_Lock success!\n");}
				break;
			case MD_UNLOCK:
				if(pthread_mutex_unlock(&my_mutex->p_mutex) == 0)
				{
					printf("Unlock success! put zz_mutex back\n");
					put_zz_mutex(my_mutex);
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
		show_mutex_hash();
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
	mutex_hash_init();
	show_mutex_hash();
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
