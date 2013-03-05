#include"glob.h"
#include"sn_sckt.h"
#include"utility.h"
#include"xml_msg.h"
#if 0
/* not used any more */
static u8 xml_fifo[XML_FILE_NAME_LEN];
void rm_xml_fifo_at_exit(void){unlink(xml_fifo);}
#endif
u32 sctp_send_sock_recv_rpl(u8 *sn_ip,SOCK_MSG *sock_msg,RPL_MSG *rpl_msg)
{
	u32 ret = 0;
	u8 buf[XML_BUFSZ];
	u32 sock_fd;
	struct sockaddr_in servaddr;
	struct sctp_event_subscribe events;
	struct sockaddr_in peeraddr;
//	struct sctp_sndrcvinfo sri;
	u32 len;
	u32 xml_msg_sz;
	u32 msg_flags = 0;
	sock_fd = socket(AF_INET,SOCK_SEQPACKET,IPPROTO_SCTP);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(FS_SUPERNODE_PORT);
	inet_pton(AF_INET,sn_ip,&servaddr.sin_addr);
	bzero(&events,sizeof(events));
	events.sctp_data_io_event = 1;
	setsockopt(sock_fd,IPPROTO_SCTP,SCTP_EVENTS,&events,sizeof(events));
	/*------------------------------- convert sock_msg to buf start -------------------------------------*/
#if 1
	if(sock_msg_to_buf(sock_msg,buf,XML_BUFSZ) != 0){
		fprintf(stderr,"sock_msg_to_buf fail!\n");
		ret = 1;
		goto op_over;
	}
#endif
	/*------------------------------- convert sock_msg to buf end -------------------------------------*/
#if 0
	bzero(&sri,sizeof(sri));

	sri.sinfo_stream = strtol(&sendline[1],NULL,0);
#endif
	xml_msg_sz = strlen(buf);
	if(sctp_sendmsg(sock_fd, buf, xml_msg_sz, 
			(struct sockaddr *)&servaddr, sizeof(servaddr), 
			0, 0,
			0/*sri.sinfo_stream*/,
			0, 0) == -1){
		ret = 2;
		perror("sctp_sendmsg");
		fprintf(stderr,"sctp_sendmsg error!\n");
		goto op_over;
	}
	len = sizeof(peeraddr);
	bzero(buf,XML_BUFSZ);
	if((xml_msg_sz = sctp_recvmsg(sock_fd, buf, XML_BUFSZ,
			(struct sockaddr*)&peeraddr, &len,
			NULL/*&sri*/,&msg_flags)) == -1){
		ret = 3;
		perror("sctp_recvmsg");
		fprintf(stderr,"sctp_recvmsg error!\n");
		goto op_over;
	}
#if 0
	printf("%d bytes received!\n",xml_msg_sz);
	printf("------- rpl_msg_start -------\n");
	printf("%s\n",buf);
	printf("-------  rpl_msg_end  -------\n");
#endif
	/*----------------------- convert xml_buf to rpl_msg start --------------------------*/
#if 0
	if(buf_to_rpl_msg(rpl_msg,buf,XML_BUFSZ) != 0){
		fprintf(stderr,"buf_to_rpl_msg fail!\n");
		ret = 4;
	}
#endif
#if 1
	if(buf_to_rpl_msg_parse_from_fd(rpl_msg,buf,XML_BUFSZ) != 0){
		fprintf(stderr,"buf_to_rpl_msg_parse_from_fd fail!\n");
		ret = 4;
	}
#endif
	/*----------------------- convert xml_buf to rpl_msg end --------------------------*/
op_over:
	close(sock_fd);
	return ret;
}
int main()
{
	SOCK_MSG sockmsg;
	RPL_MSG rplmsg;
#if 0
	/* not used any more */
	bzero(xml_fifo,XML_FILE_NAME_LEN);
	snprintf(xml_fifo,XML_FILE_NAME_LEN,"%d.xml_fifo",getpid());
	if(atexit(rm_xml_fifo_at_exit) != 0){
		perror("atexit");
	}
#endif
	printf("client process -- %d\n",getpid());
	sockmsg.type = SOCKMSG_TYPE_WRITE;
	strcpy(sockmsg.file_name,"some might say.mp3");
	strcpy(sockmsg.dest_ip,FS_SUPERNODE_IP);
	if(sctp_send_sock_recv_rpl(sockmsg.dest_ip,&sockmsg,&rplmsg) != 0){
		fprintf(stderr,"sctp_send_sock_recv_rpl fail!\n");
		return 1;
	}
	if(rplmsg.err != RPL_OK){
		fprintf(stderr,"rplmsg says some err happened!\n");
		return 2;
	}
	printf("EVERYTHING IS JUST FINE!\n");
	return 0;
}
