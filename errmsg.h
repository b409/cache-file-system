#ifndef _ERR_MSG_H
#define _ERR_MSG_H
extern u8 err_msg[];
/* print error message */
extern void perrmsg(u8 * msg);
/* set error message */
extern void serrmsg(const u8 * fmt,...);
#endif
