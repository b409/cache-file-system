#ifndef _XML_MSG
#define _XML_MSG
#include<libxml/parser.h>
#include<libxml/tree.h>
#define XML_STDIN_OUT_FILE  "-"
#define XML_BUFSZ 1024
/***** xml_sock_msg ******/
#define XML_SOCK_MSG_ROOT   "sock_msg"
#define XML_SOCK_MSG_TYPE   "type"
#define XML_SOCK_MSG_DST_IP "dest_ip"
#define XML_SOCK_MSG_FN     "file_name"
/***** xml_rpl_msg ******/
#define XML_RPL_MSG_ROOT    "rpl_msg"
#define XML_RPL_MSG_ERR     "err"
_PROTOTYPE(u32 buf_to_file,(u8 *buf,u32 bufsz,u8 *xml_file));
_PROTOTYPE(u32 file_to_buf,(u8 *buf,u32 bufsz,u8 * xml_file));

_PROTOTYPE(u32 rpl_msg_to_xml,(RPL_MSG *rpl_msg,u8 * xml_file));
_PROTOTYPE(u32 xml_to_rpl_msg,(u8 *xml_file,RPL_MSG *rpl_msg));
_PROTOTYPE(u32 sock_msg_to_xml,(SOCK_MSG *sock_msg,u8 *xml_file));
_PROTOTYPE(u32 xml_to_sock_msg,(u8 *xml_file,SOCK_MSG *sock_msg));

_PROTOTYPE(u32 xml_to_rpl_msg_parse_from_fd,(u32 fd,RPL_MSG *rpl_msg));
_PROTOTYPE(u32 xml_to_sock_msg_parse_from_fd,(u32 fd,SOCK_MSG *sock_msg));

_PROTOTYPE(u32 sock_msg_to_buf,(SOCK_MSG *sock_msg,u8 *buf,u32 bufsiz));
_PROTOTYPE(u32 rpl_msg_to_buf,(RPL_MSG *rpl_msg,u8 *buf,u32 bufsiz));
_PROTOTYPE(u32 buf_to_sock_msg,(SOCK_MSG *sock_msg,u8 *buf,u32 bufsiz));
_PROTOTYPE(u32 buf_to_rpl_msg,(RPL_MSG *rpl_msg,u8 *buf,u32 bufsiz));

_PROTOTYPE(u32 buf_to_sock_msg_parse_from_fd,(SOCK_MSG *sock_msg,u8 *buf,u32 bufsiz));
_PROTOTYPE(u32 buf_to_rpl_msg_parse_from_fd,(RPL_MSG *rpl_msg,u8 *buf,u32 bufsiz));
#endif
