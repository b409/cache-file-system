#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<tcrdb.h>
#include<stdbool.h>
#include<stdint.h>
#include<string.h>
#include<fcntl.h>
#define MAX_KEY 100
#define MAX_VALUE 200
#define MD_PORT 11211
#define ION_PORT 11212
#define FN_PORT 11213
#define SWPN_PORT 11214
#define IO_DATA_PORT 11215
#define LOCALHOST "127.0.0.1"
typedef enum
{
	META_DATA = 01,
	IO_NODE,
	FILE_NODE,
	SWAP_NODE,
	IO_DATA
}value_t;
typedef struct
{
	char flag;
	int i;
}z_value;
int tt_get();
int tt_out();
int tt_put();
char *iod_get();
int iod_out();
int iod_put();
