/* BY Grant Chen */
#ifndef _POSIX_FUNC
#define _POSIX_FUNC
/* create a new file with permission "mode"
 * and open it with "flags" */
_PROTOTYPE(u32 _CreateFile,(u8 *path,u32 mode,u32 flags));
/* open file with "flags" */
_PROTOTYPE(u32 _OpenFile,(u8 *path,u32 flags));
_PROTOTYPE(u32 _CloseFile,(u32 fd));
_PROTOTYPE(u32 _RemoveFile,(u8 *path));
/* read file */
_PROTOTYPE(u32 _ReadFile,(u32 fd,u8 *buf,u32 count,u64 offset));
/* write file */
_PROTOTYPE(u32 _WriteFile,(u32 fd,u8 * buf,u32 count,u64 offset));
/* 1) read directory from fd
 * 2) get the stat info of each entry in this directory 
 * 3) store these stat pointer to "struct stat **entry_stat "*/
_PROTOTYPE(u32 dirread,(u32 fd,struct stat **entry_stat));
/* get stat info for a given path */
_PROTOTYPE(struct stat * _stat,(u8 *path));
/* modify the stat info for a given path to new value " st "*/
_PROTOTYPE(u32 _wstat,(u8 * path,struct stat *st));
#endif
