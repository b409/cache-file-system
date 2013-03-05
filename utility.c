#include"glob.h"
#include"sn_sckt.h"
#include"tt_func.h"
char *eat_space(char *p)
{
	char * i = p;
	while(*i == CH_SPACE){
		i++;
	}
	return i;
}
char *eat_Nspace(char *p)
{
	char * i = p;
	while(*i != CH_SPACE){
		i++;
	}
	return i;
}
#if 0
/* these two functions are only used in the old version msg_disposition,
 * which is used to transform word to u32 or u64.
 * but now we use xml to transport msg */
u32 atoi_u32(char *p)
{
	u32 i = 0;
	u32 j;
	char * ch = p;
	while(*ch != CH_SPACE){
		printf("ch -- %0x  -- %c\n",*ch,*ch);
		if(*ch < '0' || *ch > '9'){
			fprintf(stderr,"Invalid character found in u32 string!\n");
			return -1;
		}
		j = (u32)(*ch - '0');
		i = 10*i + j;
		ch++;
	}
	return i;
}
u64 atoi_u64(char *p)
{
	u64 i = 0;
	u64 j;
	char * ch = p;
	while(*ch != CH_SPACE){
		printf("ch -- %0x  -- %c\n",*ch,*ch);
		if(*ch < '0' || *ch > '9'){
			fprintf(stderr,"Invalid character found in u64 string!\n");
			return -1;
		}
		j = (u64)(*ch - '0');
		i = 10*i + j;
		ch++;
	}
	return i;
}
#endif
/* atoi function 
 * in this version,we can dispose string which ends with '\0'
 * and transform it to u32 or u64 */
u32 atoi_u32(char *p)
{
	u32 i = 0;
	u32 j;
	char * ch = p;
	while(*ch != 00){
		if(*ch < '0' || *ch > '9'){
			fprintf(stderr,"Invalid character found in u32 string!\n");
			return -1;
		}
		j = (u32)(*ch - '0');
		i = 10*i + j;
		ch++;
	}
	return i;
}
u64 atoi_u64(char *p)
{
	u64 i = 0;
	u64 j;
	char * ch = p;
	while(*ch != 00){
		if(*ch < '0' || *ch > '9'){
			fprintf(stderr,"Invalid character found in u64 string!\n");
			return -1;
		}
		j = (u64)(*ch - '0');
		i = 10*i + j;
		ch++;
	}
	return i;
}
inline void u8_to_str(char ch,char *buf,u32 bufsiz)
{
	bzero(buf,bufsiz);
	snprintf(buf,bufsiz,"%c",ch);
}
inline void u32_to_str(u32 i,char *buf,u32 bufsiz)
{
	bzero(buf,bufsiz);
	snprintf(buf,bufsiz,"%u",i);
}
inline void u64_to_str(u64 i,char *buf,u32 bufsiz)
{
	bzero(buf,bufsiz);
	snprintf(buf,bufsiz,"%llu",i);
}
char * get_cache_path(char * path,char * cache_path)
{
	char *p = cache_path + strlen(CACHE_PATH);
	bzero(cache_path,FILE_PATH_LEN);
	strcpy(cache_path,CACHE_PATH);
	strcpy(p,path);
	return cache_path;
}
char * get_dtc_path(char * path,char * dtc_path)
{
	char *p = dtc_path + strlen(DTC_PATH);
	bzero(dtc_path,FILE_PATH_LEN);
	strcpy(dtc_path,DTC_PATH);
	strcpy(p,path);
	return dtc_path;
}
int get_path_from_fd(int fd,char * path)
{
	char buf[1024];
	pid_t pid;
	bzero(buf,1024);
	pid = getpid();
	snprintf(buf,1024,"/proc/%i/fd/%i",pid,fd);
	return readlink(buf,path,FILE_PATH_LEN);
}
int pthread_block_sig(int signo)
{
    int rt = 0;
    sigset_t st;
    sigemptyset(&st);
    sigaddset(&st,signo);
    rt = pthread_sigmask(SIG_BLOCK,&st,NULL);
    if(rt != 0)
    {
        printf("Block signal -- %d fail!\n",signo);
    }
    return rt;
}
int pthread_unblock_sig(int signo)
{
    int rt = 0;
    sigset_t st;
    sigemptyset(&st);
    sigaddset(&st,signo);
    rt = pthread_sigmask(SIG_UNBLOCK,&st,NULL);
    if(rt != 0)
    {
        printf("Unblock signal -- %d fail!\n",signo);
    }
    return rt;
}
#if 0
int init_md(char * path)
{
	int err = 0;
	char dtc_path[FILE_PATH_LEN];
	Meta_Data * md = (Meta_Data*)malloc(sizeof(Meta_Data));
	if(md == (Meta_Data*)0){
		perror("init meta_data : malloc");
		err = 1;
		return err;
	}
	bzero(md,sizeof(Meta_Data));
	md->ioq.head = 0;
	md->ioq.tail = 0;
	md->my_rep[0].cst = REP_CONSISTENT;
	md->my_rep[0].host_ip[] = "127.0.0.1";
	get_dtc_path(path,dtc_path);
	if(stat(dtc_path,&md->stat_info) != 0){
		perror("init meta_data : stat");
		err = 2;
	}
	else if(tt_put(path,md,META_DATA) != 0){
		err = 3;
	}
	free(md);
	return err;
}
#endif
Sigfunc * Signal(int signo,Sigfunc * func)
{
	struct sigaction act,oact;
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if(signo == SIGALRM){
#ifdef SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;
#endif
	}else{
#ifdef SA_RESTART
		act.sa_flags |= SA_RESTART;
#endif
	}
	if(sigaction(signo,&act,&oact) < 0){
		return SIG_ERR;
	}
	return oact.sa_handler;
}
#if 1
void do_space_in_file_name(u8 *str,u8 *dest)
{
	/* add '\'before SPACE in str
	 * ONLY FOR FILE_NAME */
	u8 *p = str;
	u8 *q = dest;
	while(*p != '\0'){
		if(*p == CH_SPACE){
			*q++ = '\\';
		}
		*q++ = *p++;
	}
	return;
}
/* msg to buf using command line pipe for relay 
 * but cannot be used in buf_to_*msg */
int rpl_msg_to_buf_with_command_line_pipe(RPL_MSG *rpl_msg,u8 *buf,u32 bufsiz)
{
	int i = 0;
	FILE *fp;
	static u8 command_line[COMMAND_LINE];
	bzero(command_line,COMMAND_LINE);
	snprintf(command_line,COMMAND_LINE,"./mk_xml_to_stdout %c",RPL_MSG_ERR);
//	printf("command_line -- %s\n",command_line);
	if((fp = popen(command_line,"r")) == NULL){
		fprintf(stderr,"rpl_msg_to_buf popen fail!\n");
		return 1;
	}
	bzero(buf,bufsiz);
	while(fgets(buf + i,bufsiz - i,fp) != NULL){i = strlen(buf);}
	pclose(fp);
	return 0;
}
int sock_msg_to_buf_with_command_line_pipe(SOCK_MSG *sock_msg,u8 *buf,u32 bufsiz)
{
	int i = 0;
	FILE *fp;
	static u8 command_line[COMMAND_LINE];
	static u8 file_name[FILE_PATH_LEN];
	bzero(file_name,FILE_PATH_LEN);
	do_space_in_file_name(SOCK_MSG_FN,file_name);
	bzero(command_line,COMMAND_LINE);
	snprintf(command_line,COMMAND_LINE,"./mk_xml_to_stdout %c %s %s",SOCK_MSG_TYPE,SOCK_MSG_DST_IP,file_name);
//	printf("command_line -- %s\n",command_line);
	if((fp = popen(command_line,"r")) == NULL){
		fprintf(stderr,"sock_msg_to_buf popen fail!\n");
		return 1;
	}
	bzero(buf,bufsiz);
	while(fgets(buf + i,bufsiz - i,fp) != NULL){i = strlen(buf);}
	pclose(fp);
	return 0;
}
#endif
