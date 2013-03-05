#ifndef _UTILITY
#define _UTILITY
_PROTOTYPE(char * eat_space,(char * p));
_PROTOTYPE(char * eat_Nspace,(char * p));
_PROTOTYPE(u32 atoi_u32,(char * p));
_PROTOTYPE(u64 atoi_u64,(char * p));
_PROTOTYPE(void get_time_str,(char * p,time_t t));
_PROTOTYPE(void get_port_str,(char * p,int port));
_PROTOTYPE(char * get_cache_path,(char * path,char * cache_path));
_PROTOTYPE(char * get_dtc_path,(char * path,char * dtc_path));
_PROTOTYPE(int get_path_from_fd,(int fd,char * path));
_PROTOTYPE(int pthread_block_sig,(int signo));
_PROTOTYPE(int pthread_unblock_sig,(int signo));
_PROTOTYPE(int init_md,(char * path));
_PROTOTYPE(void print_err_msg,(int i));
_PROTOTYPE(Sigfunc * Signal,(int signo,Sigfunc * func));
_PROTOTYPE(inline void u8_to_str,(char ch,char *buf,u32 bufsiz));
_PROTOTYPE(inline void u32_to_str,(u32 i,char *buf,u32 bufsiz));
_PROTOTYPE(inline void u64_to_str,(u64 i,char *buf,u32 bufsiz));
#if 1
_PROTOTYPE(void do_space_in_file_name,(u8 *str,u8 *dest));
_PROTOTYPE(int rpl_msg_to_buf_with_command_line_pipe,(RPL_MSG *rpl_msg,u8 *buf,u32 bufsiz));
_PROTOTYPE(int sock_msg_to_buf_with_command_line_pipe,(SOCK_MSG *sock_msg,u8 *buf,u32 bufsiz));
#endif
#endif
