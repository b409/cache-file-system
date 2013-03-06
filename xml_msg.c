#include"sn_sckt.h"
#include"utility.h"
#include"xml_msg.h"
#include"glob.h"
#if 0
u32 buf_to_file(u8 *buf,u32 bufsz,u8 *xml_file)
{
	int n;
	int ret = 0;
	int fd = open((char *)xml_file,O_WRONLY);
	if(fd == -1){
		ret = 1;
		fprintf(stderr,"open xml for write fail!\n");
		perror("buf_to_file : open for write");
		goto op_over;
	}
	if((n = write(fd,buf,strlen(buf))) == -1){
		ret = 2;
		perror("buf_to_file : write");
	}
	close(fd);
op_over:
	return ret;
}
u32 file_to_buf(u8 *buf,u32 bufsz,u8 * xml_file)
{
	int n;
	int ret = 0;
	int fd = open((char *)xml_file,O_RDONLY);
	if(fd == -1){
		ret = 1;
		perror("file_to_buf : open for read");
		goto op_over;
	}
	bzero(buf,bufsz);
	if((n = read(fd,buf,bufsz)) == -1){
		ret = 2;
		perror("file_to_buf : read");
	}
	close(fd);
op_over:
	return ret;
}
#endif
u32 rpl_msg_to_xml(RPL_MSG *rpl_msg,u8 *xml_file)
{
	u8 *std_io_file = XML_STDIN_OUT_FILE;
	u8 *dest_file = (xml_file == NULL)?std_io_file : xml_file;
	u8  buf[XML_BUFSZ];
	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL;
	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL,BAD_CAST XML_RPL_MSG_ROOT);
	xmlDocSetRootElement(doc,root_node);
	u8_to_str(RPL_MSG_ERR,buf,XML_BUFSZ);
	xmlNewChild(root_node,NULL,BAD_CAST XML_RPL_MSG_ERR,BAD_CAST buf);
	xmlSaveFormatFileEnc(dest_file,doc,"UTF-8",1);
//	xmlSaveFormatFileEnc("-",doc,"UTF-8",1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	xmlMemoryDump();
	return 0;
}
u32 sock_msg_to_xml(SOCK_MSG *sock_msg,u8 *xml_file)
{
	u8 *std_io_file = XML_STDIN_OUT_FILE;
	u8 *dest_file = (xml_file == NULL)?std_io_file : xml_file;
	u8 buf[XML_BUFSZ];
	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL;
	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL,BAD_CAST XML_SOCK_MSG_ROOT);
	xmlDocSetRootElement(doc,root_node);
	u8_to_str(SOCK_MSG_TYPE,buf,XML_BUFSZ);
	xmlNewChild(root_node,NULL,BAD_CAST XML_SOCK_MSG_TYPE,BAD_CAST buf);
	xmlNewChild(root_node,NULL,BAD_CAST XML_SOCK_MSG_DST_IP,BAD_CAST SOCK_MSG_DST_IP);
	xmlNewChild(root_node,NULL,BAD_CAST XML_SOCK_MSG_FN,BAD_CAST SOCK_MSG_FN);
	xmlSaveFormatFileEnc(dest_file,doc,"UTF-8",1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	xmlMemoryDump();
	return 0;
}
u32 xml_to_rpl_msg(u8 *xml_file,RPL_MSG *rpl_msg)
{
	u8 *std_io_file = XML_STDIN_OUT_FILE;
	u8 *src_file = (xml_file == NULL)?std_io_file : xml_file;
	u32 ret = 0;
	u8 parse_ok = 00;
	xmlDocPtr doc;
	xmlNodePtr curNode;
	xmlChar *szKey;
	if((doc = xmlReadFile(src_file,"UTF-8",XML_PARSE_RECOVER)) == NULL){
		fprintf(stderr,"DOCUMENT PARSE FAIL : %s\n",src_file);
		ret = 1;
		goto op_over;
	}
	if((curNode = xmlDocGetRootElement(doc)) == NULL){
		fprintf(stderr,"EMPTY DOCUMENT!\n");
		ret = 2;
		goto op_over;
	}
	if(xmlStrcmp(curNode->name,BAD_CAST XML_RPL_MSG_ROOT) != 0){
		fprintf(stderr,"NOT A RPL_MSG!\n");
		ret = 3;
		goto op_over;
	}
	curNode = curNode->children;
	while(curNode){
		if(!(parse_ok & RPL_ERR_PARSE_OK) && xmlStrcmp(curNode->name,(const xmlChar *)XML_RPL_MSG_ERR) == 0){
			szKey = xmlNodeGetContent(curNode);
			RPL_MSG_ERR = *(u8 *)szKey;
			xmlFree(szKey);
			parse_ok |= RPL_ERR_PARSE_OK;
			if(parse_ok == RPL_MSG_PARSE_OK)
				break;
		}
		curNode = curNode->next;
	}
op_over:
	xmlFreeDoc(doc);
	if(parse_ok != RPL_MSG_PARSE_OK){
		ret = 4;
		fprintf(stderr,"RPL_MSG PARSE FAIL!\n");
	}
	return ret;
}
u32 xml_to_sock_msg(u8 *xml_file,SOCK_MSG *sock_msg)
{
	u8 *std_io_file = XML_STDIN_OUT_FILE;
	u8 *src_file = (xml_file == NULL)?std_io_file : xml_file;
	u32 ret = 0;
	u8 parse_ok = 00;
	xmlDocPtr doc;
	xmlNodePtr curNode;
	xmlChar *szKey;
	if((doc = xmlReadFile(src_file,"UTF-8",XML_PARSE_RECOVER)) == NULL){
		fprintf(stderr,"DOCUMENT PARSE FAIL : %s\n",src_file);
		ret = 1;
		goto op_over;
	}
	if((curNode = xmlDocGetRootElement(doc)) == NULL){
		fprintf(stderr,"EMPTY DOCUMENT!\n");
		ret = 2;
		goto op_over;
	}
	if(xmlStrcmp(curNode->name,BAD_CAST XML_SOCK_MSG_ROOT) != 0){
		fprintf(stderr,"NOT A RPL_MSG!\n");
		ret = 3;
		goto op_over;
	}
	curNode = curNode->children;
	while(curNode){
		if(!(parse_ok & SOCKMSG_TYPE_PARSE_OK) && xmlStrcmp(curNode->name,(const xmlChar *)XML_SOCK_MSG_TYPE) == 0){
			szKey = xmlNodeGetContent(curNode);
			SOCK_MSG_TYPE = *(u8 *)szKey;
			xmlFree(szKey);
			parse_ok |= SOCKMSG_TYPE_PARSE_OK;
			if(parse_ok == SOCKMSG_PARSE_OK)
				break;
		}else if(!(parse_ok & SOCKMSG_DSTIP_PARSE_OK) && xmlStrcmp(curNode->name,(const xmlChar *)XML_SOCK_MSG_DST_IP) == 0){
			szKey = xmlNodeGetContent(curNode);
			strcpy(SOCK_MSG_DST_IP,szKey);
			xmlFree(szKey);
			parse_ok |= SOCKMSG_DSTIP_PARSE_OK;
			if(parse_ok == SOCKMSG_PARSE_OK)
				break;
		}else if(!(parse_ok & SOCKMSG_FN_PARSE_OK) && xmlStrcmp(curNode->name,(const xmlChar *)XML_SOCK_MSG_FN) == 0){
			szKey = xmlNodeGetContent(curNode);
			strcpy(SOCK_MSG_FN,szKey);
			xmlFree(szKey);
			parse_ok |= SOCKMSG_FN_PARSE_OK;
			if(parse_ok == SOCKMSG_PARSE_OK)
				break;
		}
		curNode = curNode->next;
	}
op_over:
	xmlFreeDoc(doc);
	if(parse_ok != SOCKMSG_PARSE_OK){
		ret = 4;
		fprintf(stderr,"SOCK_MSG PARSE FAIL!\n");
	}
	return ret;
}
/* parse xml file from file descriptor */
u32 xml_to_rpl_msg_parse_from_fd(u32 fd,RPL_MSG *rpl_msg)
{
	u32 ret = 0;
	u8 parse_ok = 00;
	xmlDocPtr doc;
	xmlNodePtr curNode;
	xmlChar *szKey;
	if((doc = xmlReadFd(fd,NULL,"UTF-8",XML_PARSE_RECOVER)) == NULL){
		fprintf(stderr,"DOCUMENT PARSE FAIL!\n");
		ret = 1;
		goto op_over;
	}
	if((curNode = xmlDocGetRootElement(doc)) == NULL){
		fprintf(stderr,"EMPTY DOCUMENT!\n");
		ret = 2;
		goto op_over;
	}
	if(xmlStrcmp(curNode->name,BAD_CAST XML_RPL_MSG_ROOT) != 0){
		fprintf(stderr,"NOT A RPL_MSG!\n");
		ret = 3;
		goto op_over;
	}
	curNode = curNode->children;
	while(curNode){
		if(!(parse_ok & RPL_ERR_PARSE_OK) && xmlStrcmp(curNode->name,(const xmlChar *)XML_RPL_MSG_ERR) == 0){
			szKey = xmlNodeGetContent(curNode);
			RPL_MSG_ERR = *(u8 *)szKey;
			xmlFree(szKey);
			parse_ok |= RPL_ERR_PARSE_OK;
			if(parse_ok == RPL_MSG_PARSE_OK)
				break;
		}
		curNode = curNode->next;
	}
op_over:
	xmlFreeDoc(doc);
	if(parse_ok != RPL_MSG_PARSE_OK){
		ret = 4;
		fprintf(stderr,"RPL_MSG PARSE FAIL!\n");
	}
	return ret;
}
u32 xml_to_sock_msg_parse_from_fd(u32 fd,SOCK_MSG *sock_msg)
{
	u32 ret = 0;
	u8 parse_ok = 00;
	xmlDocPtr doc;
	xmlNodePtr curNode;
	xmlChar *szKey;
	if((doc = xmlReadFd(fd,NULL,"UTF-8",XML_PARSE_RECOVER)) == NULL){
		fprintf(stderr,"DOCUMENT PARSE FAIL!\n");
		ret = 1;
		goto op_over;
	}
	if((curNode = xmlDocGetRootElement(doc)) == NULL){
		fprintf(stderr,"EMPTY DOCUMENT!\n");
		ret = 2;
		goto op_over;
	}
	if(xmlStrcmp(curNode->name,BAD_CAST XML_SOCK_MSG_ROOT) != 0){
		fprintf(stderr,"NOT A RPL_MSG!\n");
		ret = 3;
		goto op_over;
	}
	curNode = curNode->children;
	while(curNode){
		if(!(parse_ok & SOCKMSG_TYPE_PARSE_OK) && xmlStrcmp(curNode->name,(const xmlChar *)XML_SOCK_MSG_TYPE) == 0){
			szKey = xmlNodeGetContent(curNode);
			SOCK_MSG_TYPE = *(u8 *)szKey;
			xmlFree(szKey);
			parse_ok |= SOCKMSG_TYPE_PARSE_OK;
			if(parse_ok == SOCKMSG_PARSE_OK)
				break;
		}else if(!(parse_ok & SOCKMSG_DSTIP_PARSE_OK) && xmlStrcmp(curNode->name,(const xmlChar *)XML_SOCK_MSG_DST_IP) == 0){
			szKey = xmlNodeGetContent(curNode);
			strcpy(SOCK_MSG_DST_IP,szKey);
			xmlFree(szKey);
			parse_ok |= SOCKMSG_DSTIP_PARSE_OK;
			if(parse_ok == SOCKMSG_PARSE_OK)
				break;
		}else if(!(parse_ok & SOCKMSG_FN_PARSE_OK) && xmlStrcmp(curNode->name,(const xmlChar *)XML_SOCK_MSG_FN) == 0){
			szKey = xmlNodeGetContent(curNode);
			strcpy(SOCK_MSG_FN,szKey);
			xmlFree(szKey);
			parse_ok |= SOCKMSG_FN_PARSE_OK;
			if(parse_ok == SOCKMSG_PARSE_OK)
				break;
		}
		curNode = curNode->next;
	}
op_over:
	xmlFreeDoc(doc);
	if(parse_ok != SOCKMSG_PARSE_OK){
		ret = 4;
		fprintf(stderr,"SOCK_MSG PARSE FAIL!\n");
	}
	return ret;
}
#if 1
u32 sock_msg_to_buf(SOCK_MSG *sock_msg,u8 *buf,u32 bufsiz)
{
	u32 ret = 0;
	u32 pp[2];
	u32 stdout_fd_back_up;
	if(pipe(pp) != 0){
		perror("rpl_msg_to_buf -- pipe");
		return 1;
	}
	stdout_fd_back_up = dup(STDOUT_FILENO);
	close(STDOUT_FILENO);
	dup2(pp[1],STDOUT_FILENO);
	/* convert sock_msg to xml -- stdout */
	sock_msg_to_xml(sock_msg,NULL);
	fflush(stdout);
	/* read from pipe[0] */
	bzero(buf,bufsiz);
	if(read(pp[0],buf,bufsiz) == -1){
		perror("sock_msg_to_buf -- read pipe");
		ret = 2;
	}
	close(pp[0]);
	close(pp[1]);
	/* resume stdout */
	dup2(stdout_fd_back_up,STDOUT_FILENO);
	close(stdout_fd_back_up);
	return ret;
}
u32 rpl_msg_to_buf(RPL_MSG *rpl_msg,u8 *buf,u32 bufsiz)
{
	u32 ret = 0;
	u32 pp[2];
	u32 stdout_fd_back_up;
	if(pipe(pp) != 0){
		perror("rpl_msg_to_buf -- pipe");
		return 1;
	}
	stdout_fd_back_up = dup(STDOUT_FILENO);
	close(STDOUT_FILENO);
	dup2(pp[1],STDOUT_FILENO);
	/* convert rpl_msg to xml -- stdout */
	rpl_msg_to_xml(rpl_msg,NULL);
	fflush(stdout);
	/* read from pipe[0] */
	bzero(buf,bufsiz);
	if(read(pp[0],buf,bufsiz) == -1){
		perror("rpl_msg_to_buf -- read pipe");
		ret = 2;
	}
	close(pp[0]);
	close(pp[1]);
	/* resume stdout */
	dup2(stdout_fd_back_up,STDOUT_FILENO);
	close(stdout_fd_back_up);
	return ret;
}
u32 buf_to_sock_msg_parse_from_fd(SOCK_MSG *sock_msg,u8 *buf,u32 bufsiz)
{
	u32 ret = 0;
	u32 pp[2];
	if(pipe(pp) != 0){
		perror("buf_to_sock_msg_parse_from_fd  -- pipe");
		ret = 1;
		goto op_over;
	}
	if(write(pp[1],buf,strlen(buf)) == -1){
		perror("buf_to_sock_msg_parse_from_fd -- write pipe");
		ret = 2;
		close(pp[1]);
		goto op_over;
	}
	/* It seems like closing pp1 can prevent reading of pp0 being blocked!
	 * But I don't clearly know about how it works! orz */
	close(pp[1]);
	if(xml_to_sock_msg_parse_from_fd(pp[0],sock_msg) != 0){
		fprintf(stderr,"xml_to_sock_msg_parse_from_fd fail!\n");
		ret = 3;
	}
op_over:
	if(ret != 1){close(pp[0]);}
	return ret;
}
u32 buf_to_rpl_msg_parse_from_fd(RPL_MSG *rpl_msg,u8 *buf,u32 bufsiz)
{
	u32 ret = 0;
	u32 pp[2];
	if(pipe(pp) != 0){
		perror("buf_to_rpl_msg_parse_from_fd  -- pipe");
		ret = 1;
		goto op_over;
	}
	if(write(pp[1],buf,strlen(buf)) == -1){
		perror("buf_to_rpl_msg_parse_from_fd -- write pipe");
		ret = 2;
		close(pp[1]);
		goto op_over;
	}
	/* It seems like closing pp1 can prevent reading of pp0 being blocked!
	 * But I don't clearly know about how it works! orz */
	close(pp[1]);
	if(xml_to_rpl_msg_parse_from_fd(pp[0],rpl_msg) != 0){
		fprintf(stderr,"xml_to_rpl_msg_parse_from_fd fail!\n");
		ret = 3;
	}
op_over:
	if(ret != 1){close(pp[0]);}
	return ret;
}
/* from stdin */
u32 buf_to_sock_msg(SOCK_MSG *sock_msg,u8 *buf,u32 bufsiz)
{
	u32 ret = 0;
	u32 pp[2];
	u32 stdin_fd_back_up = dup(STDIN_FILENO);
	if(pipe(pp) != 0){
		perror("buf_to_sock_msg  -- pipe");
		ret = 1;
		goto op_over;
	}
	if(write(pp[1],buf,strlen(buf)) == -1){
		perror("buf_to_sock_msg -- write pipe");
		close(pp[1]);
		ret = 2;
		goto op_over;
	}
	close(STDIN_FILENO);
	dup2(pp[0],STDIN_FILENO);
	close(pp[1]);
	if(xml_to_sock_msg(NULL,sock_msg) != 0){
		fprintf(stderr,"xml_to_sock_msg fail!\n");
		ret = 3;
	}
op_over:
	if(ret != 1){close(pp[0]);}
	/* resume stdin */
	dup2(stdin_fd_back_up,STDIN_FILENO);
	close(stdin_fd_back_up);
	return ret;
}
u32 buf_to_rpl_msg(RPL_MSG *rpl_msg,u8 *buf,u32 bufsiz)
{
	u32 ret = 0;
	u32 pp[2];
	u32 stdin_fd_back_up = dup(STDIN_FILENO);
	if(pipe(pp) != 0){
		perror("buf_to_rpl_msg  -- pipe");
		ret = 1;
		goto op_over;
	}
	if(write(pp[1],buf,strlen(buf)) == -1){
		perror("buf_to_rpl_msg -- write pipe");
		close(pp[1]);
		ret = 2;
		goto op_over;
	}
	close(STDIN_FILENO);
	dup2(pp[0],STDIN_FILENO);
	close(pp[1]);
	if(xml_to_rpl_msg(NULL,rpl_msg) != 0){
		fprintf(stderr,"xml_to_rpl_msg fail!\n");
		ret = 3;
	}
op_over:
	if(ret != 1){close(pp[0]);}
	/* resume stdin */
	dup2(stdin_fd_back_up,STDIN_FILENO);
	close(stdin_fd_back_up);
	return ret;
}
#endif
#if 0
int main()
{
	SOCK_MSG sockmsg;
	SOCK_MSG *sock_msg = &sockmsg;
	RPL_MSG rplmsg;
	RPL_MSG *rpl_msg = &rplmsg;
	SOCK_MSG_TYPE = SOCKMSG_TYPE_UPDATE;
	strcpy(SOCK_MSG_FN,"testfile");
	sock_msg_to_xml(sock_msg,XML_FILE);
	bzero(sock_msg,sizeof(sockmsg));
	xml_to_sock_msg(XML_FILE,sock_msg);
	printf("sock_msg.type -- %c\n",SOCK_MSG_TYPE);
	printf("sock_msg.fn   -- %s\n",SOCK_MSG_FN);
	/* rpl_msg */
	RPL_MSG_ERR = RPL_ERR_OPEN;
	rpl_msg_to_xml(rpl_msg,XML_FILE);
	bzero(rpl_msg,sizeof(rplmsg));
	xml_to_rpl_msg(XML_FILE,rpl_msg);
	printf("rpl_msg.err -- %c\n",RPL_MSG_ERR);
	return 0;
}
#endif
