#include"tt_glob.h"
int get_db_port(value_t vt)
{
	int db_port;
	switch(vt)
	{
		case META_DATA:
			db_port = MD_PORT;
			break;
		case IO_NODE:
			db_port = ION_PORT;
			break;
		case FILE_NODE:
			db_port = FN_PORT;
			break;
		case SWAP_NODE:
			db_port = SWPN_PORT;
			break;
		case IO_DATA:
			db_port = IO_DATA_PORT;
			break;
		default:
			fprintf(stderr,"Unrecognized db_port!\n");
			db_port = 0;
			break;
	}
	return db_port;
}
int tt_get(char * key,char * zv,value_t vt)
{
	int ecode;
	int nv;
	int rt = 0;
	int nk = strlen(key);
	char * value;
	TCRDB * rdb = tcrdbnew();
	int db_port = get_db_port(vt);
	printf("db_port -- %d\n",db_port);
	if(!tcrdbopen(rdb,LOCALHOST,db_port))
	{
		ecode = tcrdbecode(rdb);
		fprintf(stderr,"tt_get:open %s error -- %s\n",db_port,tcrdberrmsg(ecode));
		rt = 1;
	}
	else
	{
		printf("db open success!\n");
	    if((value = tcrdbget(rdb,key,nk,&nv)) == NULL)
    	{
	    	ecode = tcrdbecode(rdb);
	    	fprintf(stderr,"(key=%s) tt_get error:%s\n",key,tcrdberrmsg(ecode));
	    	rt = 2;
    	}
    	else
    	{
	    	printf("(key=%s) tt_get success!\n",key);
			printf("%d bytes read\n",nv);
			memcpy(zv,value,nv);
			free(value);
    	}
	}
	tcrdbdel(rdb);
	return rt;
}
int tt_out(char * key,value_t vt)
{
	int ecode;
	int rt = 0;
	TCRDB * rdb = tcrdbnew();
	int db_port = get_db_port(vt);
	printf("db_port -- %d\n",db_port);
	if(!tcrdbopen(rdb,LOCALHOST,db_port))
	{
		ecode = tcrdbecode(rdb);
		fprintf(stderr,"tt_out:open %s error -- %s\n",db_port,tcrdberrmsg(ecode));
		rt = 1;
	}
	else
	{
		printf("db open success!\n");
	    if(tcrdbout2(rdb,key))
	    {
	        printf("(key=%s) tt_out success!\n",key);
	    }
	    else
	    {
	        ecode = tcrdbecode(rdb);
		    fprintf(stderr,"(key=%s) tt_out error:%s\n",key,tcrdberrmsg(ecode));
		    rt = 2;
	    }
	}
	tcrdbdel(rdb);
	return rt;
}
int tt_put(char * key,char * zv,int nv,value_t vt)
{
	int ecode;
	int rt = 0;
	int nk = strlen(key);
	TCRDB * rdb = tcrdbnew();
	int db_port = get_db_port(vt);
	printf("db_port -- %d\n",db_port);
	if(!tcrdbopen(rdb,LOCALHOST,db_port))
	{
		ecode = tcrdbecode(rdb);
		fprintf(stderr,"tt_put:open %s error -- %s\n",db_port,tcrdberrmsg(ecode));
		rt = 1;
	}
	else
	{
		printf("db open success!\n");
    	if(!tcrdbput(rdb,key,nk,zv,nv))
    	{
    		ecode = tcrdbecode(rdb);
    		fprintf(stderr,"(key=%s) tt_put error:%s\n",key,tcrdberrmsg(ecode));
    		rt = 2;
    	}
    	else
    	{
			printf("(key=%s) tt_put success!\n",key);
    	}
	}
	tcrdbdel(rdb);
	return rt;
}
#if 0
int md_get(char * path,Meta_Data *md)
{
	return tt_get(path,md,META_DATA);
}
int md_out(char *path)
{
	return tt_out(path,META_DATA);
}
int md_put(char *path,Meta_Data *md)
{
	return tt_put(path,md,sizeof(Meta_Data),META_DATA);
}
#endif
char *iod_get(char *key,int *nv)
{
	int ecode;
	int nk = strlen(key);
	char *value;
	TCRDB * rdb = tcrdbnew();
	int db_port = get_db_port(IO_DATA);
	printf("db_port -- %d\n",db_port);
	if(!tcrdbopen(rdb,LOCALHOST,db_port))
	{
		ecode = tcrdbecode(rdb);
		fprintf(stderr,"iod_get:open %s error -- %s\n",db_port,tcrdberrmsg(ecode));
		return NULL;
	}else{
		printf("db open success!\n");
	    if((value = tcrdbget(rdb,key,nk,nv)) == NULL)
    	{
	    	ecode = tcrdbecode(rdb);
	    	fprintf(stderr,"(key=%s) tt_get error:%s\n",key,tcrdberrmsg(ecode));
			return NULL;
		}else{
	    	printf("(key=%s) tt_get success!\n",key);
			printf("%d bytes read\n",*nv);
    	}
	}
	tcrdbdel(rdb);
	return value;
}
int iod_out(char *key)
{
	return  0;
}
int iod_put(char *key,char *value, int count)
{
	return tt_put(key,value,count,IO_DATA);
}
