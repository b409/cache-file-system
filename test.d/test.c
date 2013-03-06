#include"glob.h"
#include"xml_msg.h"
#include"utility.h"
#include"sn_sckt.h"
static u8 buf[BUFSIZ];
int main()
{
	RPL_MSG rpl_msg;
	rpl_msg.err = RPL_ERR_BUF_TO_XML;
	if(rpl_msg_to_buf(&rpl_msg,buf,BUFSIZ) != 0){
		fprintf(stderr,"rpl_msg_to_buf fail!\n");
		exit(1);
	}
	printf("buf -- \n%s",buf);
	exit(0);
}
