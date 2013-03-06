#include"glob.h"
#include"fs_socket.h"
int main()
{
/*	int fd = 0;
	int addr_len; 
	char buf[BUFSZ] = "hello world!";
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	inet_pton(AF_INET,FS_SUPERNODE_IP,(void*)&addr.sin_addr);
	addr.sin_port = htons(FS_SUPERNODE_PORT);
	addr_len = sizeof(addr);
	if((fd = socket(AF_INET,SOCK_STREAM,0)) == -1){
		perror("connect_supernode : socket");
	}
	if(connect(fd,(struct sockaddr*)&addr,addr_len) != 0){
		fd = -1;
		perror("connect_supernode : connect");
	}*/
	char buf[BUFSZ] = "hello world!";
	int fd = connect_super_node(FS_SUPERNODE_IP);
	printf("fd -- %d\n",fd);
	if(write(fd,buf,BUFSZ) == -1){
		perror("write");
	}
	close(fd);
	return 0;
}
