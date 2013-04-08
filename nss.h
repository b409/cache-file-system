#include"glob.h"
#define RBT_RED			'R'
#define RBT_BLACK		'B'
#define p(z)			z->p
#define l(z)			z->l
#define r(z)			z->r
#define c(z)			z->c
#define i(z)			z->i
#define RED_STR			"red"
#define BLACK_STR		"black"
#define LEFT_CHILD		01
#define PARENT			02
#define RIGHT_CHILD		03
#define H_LEFT_CHILD	"LEFT_CHILD : "
#define H_PARENT		"PARENT : "
#define H_RIGHT_CHILD	"RIGHT_CHILD : "
#define ELEMENTS_NUM	13
typedef struct RBT{
	u32 i;
	u8 c;
	struct RBT * l;
	struct RBT * r;
	struct RBT * p;
}rbt;
/********************* ACL & FILE_TYPE DEFINITION *******************
 * ACL\BUCKET&OBJ		BUCKET								OBJ
 * read					list all objs under this bucjet		read this obj
 * write				create&remove%overwrite file		not defined
 * read_acl				read the bucket acl					read the obj acl
 * write_acl			write the bucket acl				write the obj acl
 * full_control			read&writ&r_acl&w_acl				read&r_acl&w_acl
 * */
#define DIR_BIT				0x1000
#define REG_BIT				0x2000
#define SHR_FLAG_BIT		0x4000
#define SHR_DIR_BIT			0x8000
#define U_READ				0x0800
#define U_WRIT				0x0400
#define U_RACL				0x0200
#define U_WACL				0x0100
#define G_READ				0x0080
#define G_WRIT				0x0040
#define G_RACL				0x0020
#define G_WACL				0x0010
#define O_READ				0x0008
#define O_WRIT				0x0004
#define O_RACL				0x0002
#define O_WACL				0x0001
#define DIR_BIT_SET(ft)		(ft & DIR_BIT)
#define REG_BIT_SET(ft)		(ft & REG_BIT)
#define SHR_FLG_BIT_SET(ft)	(ft & SHR_FLAG_BIT)
#define SHR_DIR_BIT_SET(ft)	(ft & SHR_DIR_BIT)
#define FT_DIR(ft)			(DIR_BIT_SET(ft) && \
							 !REG_BIT_SET(ft) && \
							 !(SHR_FLG_BIT_SET(ft)&&SHR_DIR_BIT_SET(ft)))
#define FT_REG(ft)			(REG_BIT_SET(ft) &&\
							 !DIR_BIT_SET(ft) &&\
							 !SHR_FLG_BIT_SET(ft) &&\
							 !SHR_DIR_BIT_SET(ft))
#define SET_DIR(ft)			ft |= DIR_BIT
#define SET_REG(ft)			ft |= REG_BIT
#define SET_SHR_FLG(ft)		ft |= SHR_FLG_BIT
#define SET_SHR_DIR(ft)		ft |= SHR_DIR_BIT

#define HOME_FOR_SU          "/su"
#define HOME_FOR_SHARED      "/shared"

#define HOME_DIR_NAME		 "~"
#define ROOT_DIR_NAME		 "/"
#define UPPER_DIR_NAME		 ".."
#define SHARED_DIR_NAME	     "shared"
/* user's information is saved in a file */
#define USER_INFO_FILE		"user_info.txt"
/* super user info */
#define SU_UID				00
#define SU_GID				00
#define SU_NAME				"su"
#define SU_INIT_PW			"123"
/* user's info format
 * uid + gid + home_dir + user_name + password */
#define USER_INFO_FMT				"%d %d /%s %s %s\n"
#define GET_USER_INFO_CMD_FMT		"grep %s %s"  /*1st %s is user_name,2nd is user_info file */
#define GET_LEAST_AVAILABLE_UID		"./get_least_available_uid.sh"
#define USER_INFO_BUFSZ				1024
/*-------------------------------------split------------------------------------*/
typedef struct NS_NODE{
	u8 * name;                     /* file name */
	u16 flag;					   /* acl and file type */
//	u8 file_type;                  /* REGULAR_FILE || DIRECTORY_FILE || SLINK_FILE */
	u32 uid;
	u32 gid;
//	u32 acl;					   /* access control list */
	struct NS_NODE * parent;	   /* parent */
	struct NS_NODE ** child;       /* child array */
	u32 how_many_children;         /* how many children */
}ns_node;
/* cache file system user */
typedef struct CFS_USER{
	u32 uid;
	u32 gid;
	u8 c;
	struct CFS_USER * l;
	struct CFS_USER * r;
	struct CFS_USER * p;
}cfs_user;
