#include"glob.h"
#include"fs_socket.h"
#include"sock_msg.h"
int _CreateFile(char * path,u32 perm,int mode)
{
	char buf[BUFSIZ];
	int fd;
	/* First of all,get host ip via checking metadata */
//	char * ip = get_host_ip(path);
//	int fd = connect_super_node(ip);
	/* send create_file OCR_MSG to super node */
	OCR_MSG ocrmsg;
	OCR_MSG * ocr_msg = &ocrmsg;
	RPL_MSG rplmsg;
	RPL_MSG * rpl_msg = &rplmsg;
	int n = strlen(path);
	if(n > FILE_PATH_LEN){
		fprintf(stderr,"path too long!\n");
		return -1;
	}
	OCR_MSG_TYPE = OCR_CREAT;
	strncpy(OCR_MSG_FILE_NAME,path,n);
    OCR_MSG_PERM = perm;
	OCR_MSG_MODE = mode;
	/* INIT RPL_MSG */
	RPL_ERRNO = RPL_ERR_CREAT;
	RPL_COUNT = 0;
	if((fd = connect_super_node(FS_SUPERNODE_IP)) == -1){
		fprintf(stderr,"connect_super_node fail!\n");
		goto op_over;
	}
	if(send_ocr_msg(fd,ocr_msg,buf,BUFSIZ) != 0){
		fprintf(stderr,"send_ocr_msg fail!\n");
		close(fd);
		fd = -1;
		goto op_over;
	}
	if(recv_rpl_msg(fd,rpl_msg,buf,BUFSIZ) != 0){
		fprintf(stderr,"recv_rpl_msg fail!\n");
		close(fd);
		fd = -1;
		goto op_over;
	}
	if(RPL_ERRNO != RPL_OK){
		close(fd);
		fd = -1;
	}
op_over:
	return fd;
}
int _OpenFile(char * path,int mode)
{ 
	char buf[BUFSIZ];
	int fd;
	/* First of all,get host ip via checking metadata */
//	char * ip = get_host_ip(path);
//	int fd = connect_super_node(ip);
	/* send create_file OCR_MSG to super node */
	OCR_MSG ocrmsg;
	OCR_MSG * ocr_msg = &ocrmsg;
	RPL_MSG rplmsg;
	RPL_MSG * rpl_msg = &rplmsg;
	int n = strlen(path);
	if(n > FILE_PATH_LEN){
		fprintf(stderr,"path too long!\n");
		return -1;
	}
	OCR_MSG_TYPE = OCR_CREAT;
	strncpy(OCR_MSG_FILE_NAME,path,n);
    OCR_MSG_PERM = 0;
	OCR_MSG_MODE = mode;
	/* INIT RPL_MSG */
	RPL_ERRNO = RPL_ERR_CREAT;
	RPL_COUNT = 0;
	if((fd = connect_super_node(FS_SUPERNODE_IP)) == -1){
		fprintf(stderr,"connect_super_node fail!\n");
		goto op_over;
	}
	if(send_ocr_msg(fd,ocr_msg,buf,BUFSIZ) != 0){
		fprintf(stderr,"send_ocr_msg fail!\n");
		close(fd);
		fd = -1;
		goto op_over;
	}
	if(recv_rpl_msg(fd,rpl_msg,buf,BUFSIZ) != 0){
		fprintf(stderr,"recv_rpl_msg fail!\n");
		close(fd);
		fd = -1;
		goto op_over;
	}
	if(RPL_ERRNO != RPL_OK){
		close(fd);
		fd = -1;
	}
op_over:
	return fd;
} 
int _CloseFile(int fd)
{
	/* send close msg 
	 * then close fd */
	char buf[BUFSIZ];
	int err = 0;
	IO_MSG iomsg;
	IO_MSG * io_msg = &iomsg;
	IO_MSG_TYPE = IO_CLOSE;
	IO_MSG_COUNT = 0;
	IO_MSG_OFFSET = 0;
	if(send_io_msg(fd,io_msg,buf,BUFSIZ) != 0){
		fprintf(stderr,"send_io_msg fail!\n");
		err = 1;
	}
	return err;
}
int _RemoveFile(char * path)
{
	char buf[BUFSIZ];
	int err = 0;
	int fd;
	RPL_MSG rplmsg;
	RPL_MSG * rpl_msg = &rplmsg;
	OCR_MSG ocrmsg;
	OCR_MSG * ocr_msg = &ocrmsg;
	int n = strlen(path);
	if(n > FILE_PATH_LEN){
		fprintf(stderr,"path too long!\n");
		return -1;
	}
	OCR_MSG_TYPE = OCR_REMOVE;
	OCR_MSG_PERM = 0;
	OCR_MSG_MODE = 0;
	strncpy(OCR_MSG_FILE_NAME,path,n);
	if((fd = connect_super_node(FS_SUPERNODE_IP)) == -1){
		fprintf(stderr,"connect_super_node fail!\n");
		err = 1;
		goto op_over;
	}
	if(send_ocr_msg(fd,ocr_msg,buf,BUFSIZ) != 0){
		fprintf(stderr,"send_ocr_msg fail!\n");
		err = 2;
		goto op_over;
	}
	if(recv_rpl_msg(fd,rpl_msg,buf,BUFSIZ) != 0){
		fprintf(stderr,"send_ocr_msg fail!\n");
		err = 3;
	}
op_over:
	if(RPL_ERRNO != RPL_OK){
		err = 4;
	}
	if(fd){close(fd);}
	return err;
}
int _ReadFile(int fd,u8 * buf,u32 count,u64 offset)
{
	int err;
	char msg_buf[BUFSIZ];
	RPL_MSG rplmsg;
	RPL_MSG * rpl_msg = &rplmsg;
	IO_MSG iomsg;
	IO_MSG * io_msg = &iomsg;
	IO_MSG_TYPE = IO_READ;
	IO_MSG_COUNT = count;
	IO_MSG_OFFSET = offset;
	if(send_io_msg(fd,io_msg,msg_buf,BUFSIZ) != 0){
		err = -1;
		goto op_over;
	}
	if(recv_rpl_msg(fd,rpl_msg,msg_buf,BUFSIZ) != 0){
		err = -1;
		goto op_over;
	}
	printf("RPL_COUNT == %d\n",RPL_COUNT);
	if(RPL_ERRNO == RPL_OK){
		if((err = recv_buf_msg(fd,buf,count)) <= 0){
			fprintf(stderr,"recv_buf_msg : get data fail@!\n");
		}
	}
op_over:
	return err;
}
int _WriteFile(int fd,u8 * buf,u32 count,u64 offset)
{
	int err;
	char msg_buf[BUFSIZ];
	RPL_MSG rplmsg;
	RPL_MSG * rpl_msg = &rplmsg;
	IO_MSG iomsg;
	IO_MSG * io_msg = &iomsg;
	IO_MSG_TYPE = IO_READ;
	IO_MSG_COUNT = count;
	IO_MSG_OFFSET = offset;
	if(send_io_msg(fd,io_msg,msg_buf,BUFSIZ) != 0){
		err = -1;
		goto op_over;
	}
	if(send_buf_msg(fd,buf,count) <= 0){
		fprintf(stderr,"send_buf_msg fail!\n");
		err = -1;
		goto op_over;
	}
	if(recv_rpl_msg(fd,rpl_msg,msg_buf,BUFSIZ) != 0){
		fprintf(stderr,"recv_rpl_msg fail@!\n");
		err = -1;
		goto op_over;
	}
	if(RPL_ERRNO == RPL_OK){
		err = RPL_COUNT;
	}
op_over:
	return err;
}
int dirread(int fd,struct stat **stat)
{
}
struct stat * _stat(char * path)
{
}
int _wstat(char * path,struct stat * st)
{
}

