/* file   : glob.h 
 * author : grant chen 
 * date   : Jan 6 2013
 * */
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<pthread.h>
#include<errno.h>
#include<signal.h>
#include<inttypes.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<time.h>
#include<stdbool.h>
#include<stdint.h>
#include<tcrdb.h>
#define CH_SPACE            0x20
#define FILE_PATH_LEN       260    /* file_name length */
#define IO_NODE_KEY_LEN     260  /* IO_Node key length */
#define IO_Q_LEN            50          /* I/O on a file is lined in queue */ 
#define REPLICA_NUM         3        /* Replica number */
/* replica consistent stat */
#define REP_CONSISTENT      00    /* replica is in consistent stat */
#define REP_NONCONSISTENT   01 /* Inconsistent */
/* dirty flag */
#define O_CLEAN             00           /* cache file is clean */ 
#define O_DIRTY             01           /* dirty , need to be flushed to dt_center */
/* DTC or CACHE file */
#define DTC_FILE            00          /* dtc_file is taken as a replica with a DTC_FILE flag */
#define CACHE_FILE          01        /* replica is a cache_file */
#define _PROTOTYPE(function,params) function()
#define CACHE_PATH          "/mnt/supercache"   /* cache files are placed in this directory */
#define DTC_PATH            "/mnt/dtcenter"     /* dtc file */
#define bzero(p,n)          memset(p,0,n)
/* type definition */
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#if 0
/* define fifo for xml_file */
#define XML_FIFO_NAME_SZ    10
typedef u8 *  XML_FIFO;
#endif
typedef void Sigfunc(int);
int main(int argc,char *argv[])
{
	int i = 0;
	char *p = argv[1];
	printf("argv1 -- %s\n",argv[1]);
	while(*p != '\0'){
		printf("%d -- %c\n",i++,*p++);
	}
	printf("\n");
	return 0;
}
