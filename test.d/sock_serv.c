#include"glob.h"
#include"fs_socket.h"
int main()
{
	char s[BUFSZ];
	char clnt_ip[INET_ADDRSTRLEN];
	int clnt_port;
	int listen_fd,connect_fd,addr_len,clnt_addr_len,n;
	struct sockaddr_in addr,clnt_addr;
	addr_len = sizeof(addr);
	bzero(&addr,addr_len);
	addr.sin_family = AF_INET;
	inet_pton(AF_INET,FS_SUPERNODE_IP,(void*)&addr.sin_addr);
	addr.sin_port = htons(FS_SUPERNODE_PORT);
	listen_fd = socket(AF_INET,SOCK_STREAM,0);
	if(bind(listen_fd,(struct sockaddr*)&addr,addr_len) == -1){
		perror("bind");
	}
	listen(listen_fd,FS_SUPERNODE_MAX_LISTEN_Q);
	for(;;)
	{
		printf("server listening...\n");
		bzero(clnt_ip,INET_ADDRSTRLEN);
		connect_fd = accept(listen_fd,(struct sockaddr*)&clnt_addr,&clnt_addr_len);
		printf("connect_fd -- %d\n",connect_fd);
		inet_ntop(AF_INET,(void *)&clnt_addr.sin_addr,clnt_ip,INET_ADDRSTRLEN);
		clnt_port = ntohs(clnt_addr.sin_port);
		printf("connection from host -- ip   : %s\n",clnt_ip);
		printf("                        port : %d\n",clnt_port);
		if(read(connect_fd,s,BUFSZ) == -1){
			perror("read");
		}else{
			printf("received data -- %s\n",s);
		}
		close(connect_fd);
	}
	return 0;
}
