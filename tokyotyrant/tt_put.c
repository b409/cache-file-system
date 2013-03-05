#include"tt_glob.h"
int main(int argc,char * argv[])
{
	int rt = 0;
	char * key;
	z_value * zv;
	if(argc != 2)
	{
		fprintf(stderr,"Invalid arguments\n");
		rt = 1;
		return rt;
	}
	key = argv[1];
	zv = (z_value*)malloc(sizeof(z_value));
	zv->flag = 0;
	zv->i = 100;
	if(tt_put(key,zv,sizeof(z_value),META_DATA) == 0)
	{
		printf("TT_PUT SUCCESS!\n");
	}
	else{rt = 2;}
	return rt;
}
