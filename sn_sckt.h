/* file   : sn_sckt.h 
 * author : grant chen 
 * date   : Jan 6 2013
 * */
#ifndef _SN_SCKT
#define _SN_SCKT
#include"glob.h"
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netinet/sctp.h>
#define FS_SUPERNODE_IP "127.0.0.1"
#define FS_SUPERNODE_PORT 9399
#define FS_SUPERNODE_MAX_LISTEN_Q 100
/*----------------------- sock_msg dispose definition start ------------------------*/
/* reply error message */
#define RPL_OK						'G'
#define RPL_ERR_UNLINK				'H'
#define RPL_ERR_TRUNC				'I'
#define RPL_ERR_UPDATE_REP		    'J'
#define RPL_ERR_REMOVE_REP          'K'
#define RPL_ERR_UNDEFINED_MSG_TYPE  'L'
/* msg_type 
 * Operations which will lead to data inconsistency */
#define SOCKMSG_TYPE_WRITE						'M'
#define SOCKMSG_TYPE_REMOVE						'N'
#define SOCKMSG_TYPE_OPEN_WITH_OTRUNC			'O'
#define SOCKMSG_TYPE_CREAT_ON_EXIST				'P'
/* msg parser flag */
#define SOCKMSG_TYPE_PARSE_OK   (1 << 0)
#define SOCKMSG_DSTIP_PARSE_OK  (1 << 1)
#define SOCKMSG_FN_PARSE_OK     (1 << 2)
#define SOCKMSG_PARSE_OK        (SOCKMSG_TYPE_PARSE_OK | SOCKMSG_DSTIP_PARSE_OK | SOCKMSG_FN_PARSE_OK)
/* rpl_msg parser flag */
#define RPL_ERR_PARSE_OK        (1 << 0)
#define RPL_MSG_PARSE_OK        (RPL_ERR_PARSE_OK)
/*----------------------- sock_msg dispose definition end ------------------------*/
/* sock_msg definition */
typedef struct{
	u8 type; //MSG_TYPE_UPDATE
	u8 dest_ip[INET_ADDRSTRLEN];
	u8 file_name[FILE_PATH_LEN];
}SOCK_MSG;
/* reply from super node */
typedef struct{
	u8 err; //ok OR errno
}RPL_MSG;
#define SOCK_MSG_TYPE   sock_msg->type
#define SOCK_MSG_FN     sock_msg->file_name
#define SOCK_MSG_DST_IP sock_msg->dest_ip
#define RPL_MSG_ERR     rpl_msg->err
_PROTOTYPE(u32 sctp_send_sock_recv_rpl,(u8 *sn_ip,SOCK_MSG *sock_msg,RPL_MSG *rpl_msg));
#endif
