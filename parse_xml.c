#include"glob.h"
#include"xml_msg.h"
#include"utility.h"
#include"sn_sckt.h"
int main(int argc, char *argv[])
{
	SOCK_MSG sockmsg;
	RPL_MSG rplmsg;
	if(*(char *)argv[1] == 'R'){
		if(xml_to_rpl_msg(NULL,&rplmsg) == 0){
			printf("parse_xml_from_stdin -- rpl_err : %c\n",rplmsg.err);
		}
	}else if(*(char *)argv[1] == 'S'){
		if(xml_to_sock_msg(NULL,&sockmsg) == 0){
			printf("parse_xml_from_stdin -- sock_type : %c\n",sockmsg.type);
			printf("parse_xml_from_stdin -- dest_ip   : %s\n",sockmsg.dest_ip);
			printf("parse_xml_from_stdin -- file_name : %s\n",sockmsg.file_name);
		}
	}
	return 0;
}
