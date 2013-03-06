#define MUTEX_NUM 8
#define NHASH 101
#define MTX_TAKEN_S "MTX_TAKEN"
#define MTX_FREE_S "MTX_FREE"
/* mutex.flag */
//#define MTX_TAKEN 01
//#define MTX_FREE 00
typedef struct
{
	char op_type;
	int client_sock_fd;
	char file_name[MAX_PATH];
}thread_arg;
typedef struct
{
//	char flag; /* is this mutex allocated or not */
	int count; /* how many threads are using this mutex */
	pthread_mutex_t p_mutex;
	char file_name[MAX_PATH];
}z_mutex;
typedef struct _mutex
{
//	char flag; /* is this mutex allocated or not */
	int count; /* how many threads are using this mutex */
	pthread_mutex_t p_mutex;
	char file_name[MAX_PATH];
	struct _mutex * pre;
	struct _mutex * next;
}zz_mutex;
