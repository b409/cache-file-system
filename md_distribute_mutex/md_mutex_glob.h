#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define MD_MUTEX_SERV_PORT 9378
#define MD_MUTEX_SERV_IP "127.0.0.1"
#define MD_MUTEX_SERV_MAX_LISTEN_Q 100
#define MAX_PATH 260
/* request type */
typedef enum
{
	MD_LOCK = 0,
	MD_TRY_LOCK,
	MD_UNLOCK,
}op_t;
/* response.ok_or_not */
#define OK 01
#define NOT_OK 00
/* response.why fail*/
typedef enum
{ 
	GLOB_MUTEX_LOCK_FAIL = 0,
	GLOB_MUTEX_UNLOCK_FAIL,
	ALLOC_MUTEX_FAIL,
	LOCK_FAIL,
	TRY_LOCK_FAIL,
	UNLOCK_FAIL,
	UNRECOGNIZED_OP_TYPE 
}fail_t;
typedef struct
{
	op_t op_type;
	char file_name[MAX_PATH];
}md_mutex_req_packet;
typedef struct
{
	char ok_or_not;
	fail_t why_fail;
}md_mutex_res_packet;
