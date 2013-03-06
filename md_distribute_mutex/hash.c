#include"md_mutex_glob.h"
#include"md_mutex_server.h"
unsigned int hash_func(char * str)
{
	unsigned int seed = 131;
	unsigned int hash = 0;
	unsigned char * p = (unsigned char *)str;
	while(*p != '\0')
	{
		hash = hash * seed + (*p++);
	}
	return hash % NHASH;
}
