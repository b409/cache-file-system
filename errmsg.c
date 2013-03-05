/* my own error message reporter 
 * compile this file to a shared object */
#include"glob.h"
u8 err_msg[ERR_MSG_LEN];
void perrmsg(u8 * msg)
{
	fprintf(stderr,"%s:%s\n",msg,err_msg);
	return;
}
void serrmsg(const u8 * fmt,...)
{
	va_list ap;
	va_start(ap,fmt);
	vsnprintf(err_msg,ERR_MSG_LEN,fmt,ap);
	va_end(ap);
	return;
}
