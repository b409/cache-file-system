#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"type.h"
#include"const.h"
#define FS_SUPERNODE_IP "127.0.0.1"
#define FS_SUPERNODE_PORT 9399
#define FS_SUPERNODE_MAX_LISTEN_Q 100
/* cache fs request message */
typedef struct{
	u8 io_type;//IO_READ or IO_WRITE or IO_CLOSE
	u32 count;
	u64 offset;
}IO_MSG;
typedef struct{
	u8 ocr_type; //OCR_CREAT or OCR_OPEN OCR_REMOVE
	u8 file_name[FILE_PATH_LEN];
	u32 perm;
	u32 mode;
}OCR_MSG;
/* reply from super node */
typedef struct{
	u32 errno;
	u32 count;//bytes read or write
}Reply_MSG;
