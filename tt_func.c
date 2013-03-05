/*
 * changed by Jin(2013-03-05)
 * <change> in iod_get function (marked in the function)
 * <change> u32 md_get(u8 * path,Meta_Data *md) change Meta_Data to u8
 */
#include"glob.h"
#include"tt_func.h"
#include"md_type.h"
u32 get_db_port(value_t vt)
{
	u32 db_port;
	switch(vt){
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
			fprintf(stderr,"UNRECOGNIZED VALUE TYPE!\n");
			db_port = 0;
			break;
	}
	return db_port;
}
u32 tt_get(u8 *key,u8 *zv,value_t vt)
{
	u32 ecode;
	u32 nv;
	u32 rt = 0;
	u32 nk = strlen(key);
	u8 * value;
	TCRDB * rdb = tcrdbnew();
	u32 db_port = get_db_port(vt);
	printf("db_port -- %d\n",db_port);
	if(!tcrdbopen(rdb,TT_HOST_IP,db_port)){
		ecode = tcrdbecode(rdb);
		fprintf(stderr,"tt_get:open %d error -- %s\n",db_port,tcrdberrmsg(ecode));
		rt = 1;
	}else{
	    if((value = tcrdbget(rdb,key,nk,&nv)) == NULL){
	    	ecode = tcrdbecode(rdb);
	    	fprintf(stderr,"(key=%s) tt_get error:%s\n",key,tcrdberrmsg(ecode));
	    	rt = 2;
    	}else{
	    	printf("(key=%s) tt_get success!\n",key);
			printf("%d bytes read\n",nv);
			memcpy(zv,value,nv);
			free(value);
    	}
	}
	tcrdbdel(rdb);
	return rt;
}
u32 tt_out(u8 *key,value_t vt)
{
	u32 ecode;
	u32 rt = 0;
	TCRDB * rdb = tcrdbnew();
	u32 db_port = get_db_port(vt);
	printf("db_port -- %d\n",db_port);
	if(!tcrdbopen(rdb,TT_HOST_IP,db_port)){
		ecode = tcrdbecode(rdb);
		fprintf(stderr,"tt_out:open %d error -- %s\n",db_port,tcrdberrmsg(ecode));
		rt = 1;
	}else{
	    if(tcrdbout2(rdb,key)){
	        printf("(key=%s) tt_out success!\n",key);
	    }else{
	        ecode = tcrdbecode(rdb);
		    fprintf(stderr,"(key=%s) tt_out error:%s\n",key,tcrdberrmsg(ecode));
		    rt = 2;
	    }
	}
	tcrdbdel(rdb);
	return rt;
}
u32 tt_put(u8 * key,u8 *zv,u32 nv,value_t vt)
{
	u32 ecode;
	u32 rt = 0;
	u32 nk = strlen(key);
	TCRDB * rdb = tcrdbnew();
	u32 db_port = get_db_port(vt);
	printf("db_port -- %d\n",db_port);
	if(!tcrdbopen(rdb,TT_HOST_IP,db_port)){
		ecode = tcrdbecode(rdb);
		fprintf(stderr,"tt_put:open %d error -- %s\n",db_port,tcrdberrmsg(ecode));
		rt = 1;
	}else{
    	if(!tcrdbput(rdb,key,nk,zv,nv)){
    		ecode = tcrdbecode(rdb);
    		fprintf(stderr,"(key=%s) tt_put error:%s\n",key,tcrdberrmsg(ecode));
    		rt = 2;
    	}else{
			printf("(key=%s) tt_put success!\n",key);
    	}
	}
	tcrdbdel(rdb);
	return rt;
}
u32 md_get(u8 * path,u8 *md)
{
	return tt_get(path,md,META_DATA);
}
u32 md_out(u8 *path)
{
	return tt_out(path,META_DATA);
}
u32 md_put(u8 *path,u8 *md)
{
	return tt_put(path,md,sizeof(Meta_Data),META_DATA);
}
u32 ion_get(u8 * path,u8 *ion)
{
	return tt_get(path,ion,IO_NODE);
}
u32 ion_out(u8 *path)
{
	return tt_out(path,IO_NODE);
}
u32 ion_put(u8 *path,u8 *ion)
{
	return tt_put(path,ion,sizeof(IO_Node),IO_NODE);
}
u8 *iod_get(u8 *key,u32 *nv)
{
	u32 ecode;
	u32 nk = strlen(key);
	u8 *value;
	TCRDB * rdb = tcrdbnew();
	u32 db_port = get_db_port(IO_DATA);
	printf("db_port -- %d\n",db_port);
	if(!tcrdbopen(rdb,TT_HOST_IP,db_port)){
		ecode = tcrdbecode(rdb);
		fprintf(stderr,"iod_get:open %d error -- %s\n",db_port,tcrdberrmsg(ecode));
		return NULL;
	}else{
	    if((value = tcrdbget(rdb,key,nk,nv)) == NULL){//change &nv to nv
	    	ecode = tcrdbecode(rdb);
	    	fprintf(stderr,"(key=%s) iod_get error:%s\n",key,tcrdberrmsg(ecode));
			return NULL;
		}else{
	    	printf("(key=%s) iod_get success!\n",key);
			printf("%d bytes read\n",*nv);//change nv to *nv 
    	}
	}
	tcrdbdel(rdb);
	return value;
}
u32 iod_out(u8 *key)
{
	return  tt_out(key,IO_DATA);
}
u32 iod_put(u8 *key,u8 *value, u32 count)
{
	return tt_put(key,value,count,IO_DATA);
}
