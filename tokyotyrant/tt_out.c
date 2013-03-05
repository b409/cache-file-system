#include"tt_glob.h"
int main(int argc,char * argv[])
{
	int rt = 0;
	char * key;
	if(argc != 2)
	{
		fprintf(stderr,"Invalid arguments\n");
		rt = 1;
		return rt;
	}
	key = argv[1];
	if(tt_out(key,META_DATA) == 0)
	{
		printf("TT_OUT SUCCESS!\n");
	}
	else{rt = 2;}
	return rt;
}
