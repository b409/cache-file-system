#include"glob.h"
#include"utility.h"
static char buf[1024] = "sssssssssssssssssssssssssssssssssssssssssssssssss";
int main()
{
	int n;
	int fd;
	int pp[2];
	char path[1024];
	bzero(path,1024);
	get_path_from_fd(0,path);
	printf("fd 0 : %s\n",path);
	bzero(path,1024);
	get_path_from_fd(1,path);
	printf("fd 1 : %s\n",path);
	bzero(path,1024);
	get_path_from_fd(2,path);
	printf("fd 2 : %s\n",path);
	if(pipe(pp) != 0){
		perror("pipe");
		exit(1);
	}
	printf("process -- %d\n",getpid());
	bzero(path,1024);
	get_path_from_fd(pp[1],path);
	printf("fd pipe[1] %d : %s\n",pp[1],path);
#if 0
	if((fd = open(path,O_WRONLY)) == -1){
		perror("open pipe1 for write");
		exit(1);
	}
	if((n = write(fd,buf,strlen(buf))) == -1){
		perror("write pipe");
		exit(1);
	}
	printf("%d bytes written to pipe!\n",n);
	close(fd);
#endif	
	
	bzero(path,1024);
	get_path_from_fd(pp[0],path);
	printf("fd pipe[0] %d : %s\n",pp[0],path);
#if 0
	bzero(buf,1024);
	if((fd = open(path,O_RDONLY)) == -1){
		perror("open pipe0 for read");
		exit(1);
	}
	if((n = read(fd,buf,1024)) == -1){
		perror("read pipe");
		exit(1);
	}
	close(fd);
	printf("%d bytes read from pipe!\n",n);
	printf("data : %s\n",buf);
#endif
	printf("sleep for 59s...\n");
	sleep(59);
	return 0;
}
