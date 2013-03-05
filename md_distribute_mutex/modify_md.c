#include"md_mutex_glob.h"
#include"md_mutex_client.h"
#include"md_mutex_proto.h"
int main(int argc,char **argv)
{
	md_mutex_t my_mutex;
	bzero(&my_mutex,sizeof(my_mutex));
	if(argc != 2)
	{
		perror("invalid arguments\n");
		exit(EXIT_FAILURE);
	}
	strcpy(my_mutex.file_name,argv[1]);
	if(md_mutex_lock(&my_mutex) == 0)
	{
		printf("lock ok!\n");
	}
	else
	{
		printf("lock fail!\n");
	}
	sleep(3);
	if(md_mutex_unlock(&my_mutex) == 0)
	{
		printf("unlock ok!\n");
	}
	else
	{
		printf("unlock fail!\n");
	}
	return 0;
}
