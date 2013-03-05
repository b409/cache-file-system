#include"global.h"
_PROTOTYPE(int md_mutex_func,(md_mutex_t * md_mutex,char op_type));
_PROTOTYPE(int md_mutex_lock,(md_mutex_t * md_mutex));
_PROTOTYPE(int md_mutex_try_lock,(md_mutex_t * md_mutex));
_PROTOTYPE(int md_mutex_unlock,(md_mutex_t * md_mutex));
_PROTOTYPE(unsigned int hash_func,(char * str));
