#include"tt_glob.h"
struct test_s{
	int i;
	int j;
};
int main(int argc,char *argv[])
{
	struct test_s s;
	char op   = *argv[1];
	char *key = argv[2];
	char *value;
	int len;
	s.i = 0;
	s.j = 1;
	switch(op){
		case 'G':
			if((value = iod_get(key,&len)) != NULL){
				printf("%d bytes get from tt\n",len);
				printf("value.i %d\n",((struct test_s*)value)->i);
				printf("value.j %d\n",((struct test_s*)value)->j);
			}
			break;
		case 'P':
			if(iod_put(key,&s,sizeof(s)) == 0){
				printf("put OK\n");
			}else{
				printf("put fail\n");
			}
			break;
	}
	return 0;
}
