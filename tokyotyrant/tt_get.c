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
	if(tt_get(key,zv,META_DATA) == 0)
	{
		printf("TT_GET SUCCESS!\n");
		printf("zv->flag = %d\n",zv->flag);
		printf("zv->i = %d\n",zv->i);
	}
	else{rt = 2;}
	free(zv);
	return rt;
}
