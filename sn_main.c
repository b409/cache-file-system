/* sn_main.c by Grant Chen
 * 1) The main process receives XML format sock_msg and parse it to SOCK_MSG struct.
 * 2) Child process is forked to deal with this sock_msg.
 * 3) Considering that more than one sock_msg for the same file received by this super_node,
 *    a sock_msg task queue is needed to register these sock_msgS,
 *    so that we can ensure this replica is updated in the order of sock_msg received.
 * 4) The message needed by child process is acturally the combination of SOCK_MSG and client_sock_addr,
 *    which is defined to a FIFO_MSG.
 * 5) FIFO_MSG is delievered to child process via FIFO.
 * 6) Child process received FIFO_MSG add it to task_queue. */
#include"glob.h"
#include"md_type.h"
#include"tt_func.h"
#include"sn_sckt.h"
#include"utility.h"
#include"xml_msg.h"
#if 0
/* not used any more */
static u8 xml_file[XML_FILE_NAME_LEN];
void rm_xml_file_at_exit(void){unlink(xml_file);}
#endif
/* 1) FIFO_MSG for child process. 
 * 2) Delievered via FIFO. */
typedef struct _FIFO_MSG{
	/* next & pre ptr,
	 * to make a FIFO_MSG queue. */
	struct _FIFO_MSG * next;
	struct _FIFO_MSG * pre;
	u32 len;                        /* client addr len */
	struct sockaddr_in cli_addr;    /* client addr     */
	struct sctp_sndrcvinfo sndrcv_info;			/* send_recv_info  */
	SOCK_MSG sock_msg;				/* sock_msg        */
}FIFO_MSG;
void sig_chld(int signo)
{
	pid_t pid;
	int stat;
	while((pid = waitpid(-1,&stat,WNOHANG)) > 0){
		printf("child -- %d -- terminated!\n",pid);
	}
	return;
}
/* These do_XXX functions do all the things needed,
 * to make replica to a consistent state. */
u32 do_update_replica(SOCK_MSG *sock_msg,RPL_MSG *rpl_msg)
{
	/* A write operation happened on one of the other replicas,
	 * update replica on this SN to consistent stat. */
	u32 ret = 0;
	u8 cache_path[FILE_PATH_LEN];
	get_cache_path(SOCK_MSG_FN,cache_path);
	return ret;
}
u32 do_remove_replica(SOCK_MSG *sock_msg,RPL_MSG *rpl_msg)
{
	/* file removed,all replicas should be removed. 
	 * Meta_Data of this file should be removed by the msg_sender. */
	u32 ret = 0;
	u8 cache_path[FILE_PATH_LEN];
	get_cache_path(SOCK_MSG_FN,cache_path);
	if(unlink(cache_path) != 0){
		RPL_MSG_ERR = RPL_ERR_UNLINK;
		ret = 1;
	}
	return ret;
}
u32 do_trunc_replica(SOCK_MSG *sock_msg,RPL_MSG *rpl_msg)
{
	/* one replica is opened with O_TRUNC flag,
	 * all the other replica should also be truncated. */
	u32 ret = 0;
	u8 cache_path[FILE_PATH_LEN];
	get_cache_path(SOCK_MSG_FN,cache_path);
	if(truncate(cache_path,0) != 0){
		RPL_MSG_ERR = RPL_ERR_TRUNC;
		ret = 1;
	}
	return ret;
}
u32 do_creat_on_exist(SOCK_MSG *sock_msg)
{
	/* one replica already exists in a SN,
	 * but got created with a new permission mode and truncated to 0.
	 * So all the replica should be removed and created with new permission mode. */
	u32 ret = 0;
	return ret;
}
u32 do_sock_msg(SOCK_MSG *sock_msg,RPL_MSG *rpl_msg)
{
	/* do_sock_msg:
	 * deal with sock_msg and generate rpl_msg */
	u32 ret = 0;
	/* for test */
	RPL_MSG_ERR = RPL_OK;;
	return ret;
}
int main()
{
	u32 i = 0;
	/* msg definition */
	SOCK_MSG sockmsg;
	RPL_MSG  rplmsg;
	SOCK_MSG *sock_msg = &sockmsg;
	RPL_MSG  *rpl_msg = &rplmsg;
	pid_t pid;
#if 0
	/* for xml file
	 * not used any more */
	u32 xml_fd;
#endif
	/* xml_msg buf */
	u8 buf[XML_BUFSZ];
	u32 xml_msg_sz;

	/* socket definiton */
	u32 sock_fd;
	u32 msg_flags = 0;
	u32 len;
	struct sockaddr_in servaddr,cliaddr;
	struct sctp_sndrcvinfo sri;
	struct sctp_event_subscribe events;
	sock_fd = socket(AF_INET,SOCK_SEQPACKET,IPPROTO_SCTP);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(FS_SUPERNODE_PORT);
	bind(sock_fd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	bzero(&events,sizeof(events));
	events.sctp_data_io_event = 1;
	setsockopt(sock_fd,IPPROTO_SCTP,SCTP_EVENTS,&events,sizeof(events));
	listen(sock_fd,FS_SUPERNODE_MAX_LISTEN_Q);
	Signal(SIGCHLD,sig_chld);/* to collect zombie child process */
	for(;;){
		/* main process :
		 * 1) receive xml_msg
		 * 2) fork a child process to deal with this xml_msg
		 */
		printf("main process : iteration -- %d\n",i++);
		len = sizeof(struct sockaddr_in);
		bzero(buf,XML_BUFSZ);
		if((xml_msg_sz = sctp_recvmsg(sock_fd,buf,XML_BUFSZ,
				(struct sockaddr*)&cliaddr,&len,&sri,&msg_flags)) == -1){
			if(errno == EINTR)
				continue;
			perror("sctp_recvmsg");
		}
		/* dispose msg in child process */
		if((pid = fork()) == 0){
			/* in child process :
			 * 1) convert xml_msg to sock_msg 
			 * 2) call do_sock_msg deal with sock_msg and generate rpl_msg 
			 * 3) convert rpl_msg to xml_msg 
			 * 4) send xml_msg */
			pid = getpid();
			printf("%d -- Now in child process,dealing with sock_msg!\n",pid);
#if 0
			printf("%s",buf);
#endif
#if 1
			/* parse_xml from file descriptor */
			if(buf_to_sock_msg_parse_from_fd(sock_msg,buf,XML_BUFSZ) != 0){
				fprintf(stderr,"buf_to_sock_msg_parse_from_fd fail!\n");
				exit(EXIT_FAILURE);
			}
#endif
#if 1
			printf("SOCK_MSG_TYPE   -- %c\n",SOCK_MSG_TYPE);
			printf("SOCK_MSG_DST_IP -- %s\n",SOCK_MSG_DST_IP);
			printf("SOCK_MSG_FN     -- %s\n",SOCK_MSG_FN);
#endif
			/*---------------------------------- convert xml_buf to sock_msg end  ---------------------------------------*/
			/*---------------------------------------- do_sock_msg start ------------------------------------*/
#if 1
			if(do_sock_msg(sock_msg,rpl_msg) != 0){
				fprintf(stderr,"do_sock_msg fail!\n");
			}
#endif
			/*---------------------------------------- do_sock_msg end  ------------------------------------*/
			/*-------------------------------- convert rpl_msg to xml_buf start ------------------------------*/
			if(rpl_msg_to_buf(rpl_msg,buf,XML_BUFSZ) != 0){
				fprintf(stderr,"buf_to_rpl_msg fail!\n");
				exit(EXIT_FAILURE);
			}
			/*-------------------------------- convert rpl_msg to xml_buf end  ------------------------------*/
			xml_msg_sz = strlen(buf);
			if(sctp_sendmsg(sock_fd,buf,xml_msg_sz,(struct sockaddr*)&cliaddr,len,
					0,0,sri.sinfo_stream,0,0) == -1){
				fprintf(stderr,"process -- %d sctp_sendmsg fail!\n",getpid());
				perror("sctp_sendmsg");
			}
			exit(EXIT_SUCCESS);
		}
	}
	return 0;
}
