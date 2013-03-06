#include"md_mutex_glob.h"
#include"md_mutex_client.h"
int md_mutex_func(md_mutex_t * md_mutex,op_t op_type)
{
	int res = 0;
	int fd,addr_len; 
	struct sockaddr_in addr;
	md_mutex_req_packet * req_pkt;
	md_mutex_res_packet * res_pkt;
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	inet_pton(AF_INET,MD_MUTEX_SERV_IP,(void*)&addr.sin_addr);
	addr.sin_port = htons(MD_MUTEX_SERV_PORT);
	addr_len = sizeof(addr);
	if((fd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}
	if((res = connect(fd,(struct sockaddr*)&addr,addr_len)) != 0)
	{
		perror("connect");
		exit(EXIT_FAILURE);
	}
	req_pkt = (md_mutex_req_packet*)malloc(sizeof(md_mutex_req_packet));
	bzero(req_pkt,sizeof(md_mutex_req_packet));
	req_pkt->op_type = op_type;
	strcpy(req_pkt->file_name,md_mutex->file_name);
	res = write(fd,req_pkt,sizeof(md_mutex_req_packet));
	free(req_pkt);
	if(res == -1)
	{
		perror("md_mutex_func try to send request");
		exit(EXIT_FAILURE);
	}
	res_pkt = (md_mutex_res_packet*)malloc(sizeof(md_mutex_res_packet));
	bzero(res_pkt,sizeof(md_mutex_res_packet));
	if((res = read(fd,res_pkt,sizeof(md_mutex_res_packet))) == -1)
	{
		perror("md_mutex_func get response from server");
		free(res_pkt);
		exit(EXIT_FAILURE);
	}
	close(fd);
	if(res_pkt->ok_or_not == OK){res = 0;}
	else if(res_pkt->ok_or_not == NOT_OK){
		printf("errorno:%d\n",res_pkt->why_fail);
		res = 1;
	}
	else{
		printf("Unrecognized response from server!\n");
		res = -1;
	}
	free(res_pkt);
	return res;
}
int md_mutex_lock(md_mutex_t * md_mutex)
{
	op_t op_type = MD_LOCK;
	return md_mutex_func(md_mutex,op_type);
}
int md_mutex_try_lock(md_mutex_t * md_mutex)
{
	op_t op_type = MD_TRY_LOCK;
	return md_mutex_func(md_mutex,op_type);
}
int md_mutex_unlock(md_mutex_t * md_mutex)
{
	op_t op_type = MD_UNLOCK;
	return md_mutex_func(md_mutex,op_type);
}
