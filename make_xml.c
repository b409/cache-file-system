#include"sn_sckt.h"
#include"utility.h"
#include"xml_msg.h"
#include"glob.h"
int mk_xml_sock_msg_to_xml(u8 *type,u8 *dest_ip,u8 *file_name)
{
	char buf[XML_BUFSZ];
	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL;
	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL,BAD_CAST XML_SOCK_MSG_ROOT);
	xmlDocSetRootElement(doc,root_node);
	xmlNewChild(root_node,NULL,BAD_CAST XML_SOCK_MSG_TYPE,BAD_CAST type);
	xmlNewChild(root_node,NULL,BAD_CAST XML_SOCK_MSG_DST_IP,BAD_CAST dest_ip);
	xmlNewChild(root_node,NULL,BAD_CAST XML_SOCK_MSG_FN,BAD_CAST file_name);
	xmlSaveFormatFileEnc("-",doc,"UTF-8",1);
//	xmlSaveFormatFileEnc("ff",doc,"UTF-8",1);
//	xmlSaveFormatFileEnc(xml_fifo,doc,"UTF-8",1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	xmlMemoryDump();
	return 0;
}
int mk_xml_rpl_msg_to_xml(u8 *err)
{
	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL;
	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL,BAD_CAST XML_RPL_MSG_ROOT);
	xmlDocSetRootElement(doc,root_node);
	xmlNewChild(root_node,NULL,BAD_CAST XML_RPL_MSG_ERR,BAD_CAST err);
	xmlSaveFormatFileEnc("-",doc,"UTF-8",1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	xmlMemoryDump();
	return 0;
}
int main(int argc,char *argv[])
{
	if(argc == 2){
		/* rpl_msg 
		 * argv[1] --> RPL_MSG_ERR */
		mk_xml_rpl_msg_to_xml(argv[1]);
	}else if(argc == 4){
		/* sock_msg 
		 * argv[1] --> SOCK_MSG_TYPE 
		 * argv[2] --> SOCK_MSG_DST_IP 
		 * argv[3] --> SOCK_MSG_FN */
		mk_xml_sock_msg_to_xml(argv[1],argv[2],argv[3]);
	}else{
		/* invalid argument */
		fprintf(stderr,"INVALID ARGUMENT!\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
