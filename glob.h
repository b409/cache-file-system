/* file   : glob.h 
 * author : grant chen 
 * date   : Jan 6 2013
 * 
 * changed by Jin 2013-03-05
 * <add> typedef  enum{READ,WRITE,REMOVE} IO_Type;
 * <add> #define INET_ADDRSTRLEN 20
 * */
#ifndef _GLOB
#define _GLOB
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
#include<stdarg.h>

#define ERR_MSG_LEN   1024
#define ONE_K               (1<<10)
#define ONE_M               (1<<20)
#define ONE_G               (1<<30)
#define INET_ADDRSTRLEN     20
#define COMMAND_LINE        512
#define CH_SPACE            0x20
#define FILE_PATH_LEN       260    /* file_name length */
#define IO_NODE_KEY_LEN     260  /* IO_Node key length */
#define IO_Q_LEN            256          /* I/O on a file is lined in queue */ 
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
#define _PROTOTYPE(function,params)		extern function()
#define CACHE_PATH          "/mnt/supercache/"   /* cache files are placed in this directory */
#define DTC_PATH            "/mnt/dtcenter/"     /* dtc file */
#define bzero(p,n)          memset(p,0,n)
/* type definition */
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef void Sigfunc(int);
typedef enum{READ=1,WRITE,REMOVE} IO_Type;
#if 0
/* define xml_fifo file name len for mk_xml and parse_xml */
/* not used any more */
#define XML_FILE_NAME_LEN   16
#define XML_FIFO_NAME_SZ    16
#endif
#endif
