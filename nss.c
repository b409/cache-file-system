/* cache file system namespace manager
 * BY grant chen 27,Feb,2013 */
#include"glob.h"
#include"utility.h"
#include"errmsg.h"
#include"nss.h"
/*-------------------------------------split------------------------------------*/
static u32 binary_seach_file(u8 *file_name,ns_node ** child,u32 low,u32 high)
{
	/* binary search file "file_name" in child array
	 * return value is the right position or the place where this new file should be inserted to */
	u32 mid;
	u32 i;
	if(high <= low){
		return low;
	}
	mid = (high + low)/2;
	if((i = strcmp(file_name,child[mid]->name)) < 0){
		high = mid -1;
	}else if(i == 0){
		/* find! */
		return mid;
	}else{
		low = mid + 1;
	}
	return binary_seach_file(file_name,child,low,high);
}
/*-------------------------------------split------------------------------------*/
u8 * get_full_path(u8 * file_name)
{
	/* return the full path of a given file_name */
	return NULL;
}
/*-------------------------------------split------------------------------------*/
u32 get_ns_node(u8 * file_path,ns_node ** nsnode,u32 * index,u8 * file_name_buf,u32 fbufsiz)
{
	/* get ns_node for a given path.
	 * *index is the file's position in the last lookup,no matter this look up succeeds or fails.
	 * RETURN TABLE :
	 *						return_value	file_name_buf		*nsnode
	 * success					0			 empty				ns_node ptr of the right file(not upper dir file)
	 * lkup_node not a dir :	1			 empty				the ptr of ns_node that leads to error (not a dir)
	 * is upper dir file   :    2			 empty			 	the ptr of the parent ns_node
	 * no such file or dir :	3			 file name			ptr of final dir in this file_path
	 * no such file or dir :	4			 inter_dir name		ptr of the last dir when "no such file or dir" happens
	 * ATTENTION : index only makes sense when return value is 0 : used to delete a file
	 *														   3 : used to make a new file
	 */
	u32 ret = 0;
	ns_node * lkup_node;
	u32 fn_depth = 0,inword = 0;
	u32 i = strlen(file_path);
	u8 * fn_head = NULL;
	u8 * fn_tail = NULL;
	u8 * p = file_path;
	u8 * tail = file_path + i;/* tail points to the one character right after the last byte in file_path */
	printf("get_ns_node for file %s\n",file_path);
	/* set the starting node
	 * 1) root dir if file_path starts with '/'
	 * 2) else current working dir */
	if(*p == '/'){
		lkup_node = &cfs_root_dir;
		inword = 0;
	}else{
		lkup_node = current_working_dir;
		fn_head = p;
		inword = 1;
		fn_depth++;
	}
	printf("start lookup from dir -- %s\n",lkup_node->name);
	for(p = file_path;p <= tail;p++){
		if(inword == 0 && p < tail && *p != '/'){
			/* just right enter word */
			fn_head = p;
			inword = 1;
			fn_depth++;
		}
		/* just right leave word */
		if(inword == 1 && (*p == '/' || p == tail)){
			/* when p == tail,POINTER p cannot be used to reference any data,
			 * just the position of a file name 's tail */
			fn_tail = p;
			inword = 0;
		}
		if(fn_head != NULL && fn_tail != NULL && fn_head < fn_tail){
			if(ret == 3){
				serrmsg("LOOKUP INTER_DIR FAIL!");
				ret = 4;
				goto op_over;
			}
			ret = 0;
			i = fn_tail - fn_head;
			bzero(file_name_buf,fbufsiz);
			strncpy(file_name_buf,fn_head,i);
			printf("now lookup file -- %s\n",file_name_buf);
		/***************************** lkup_node is not dir ****************************/
			if(!IS_DIR_FILE(lkup_node->file_type)){
				serrmsg("NOT A DIRECTORY : %s",lkup_node->name);
				ret = 1;
				goto op_over;
			}
		/******************** look up file ***********************/
			if(strcmp(file_name_buf,UPPER_DIR_NAME) == 0){
				printf("go to upper\n");
				/* go to upper dir */
				if(IS_SHARED_DIR(lkup_node->file_type)){
					/* for /shared dir,UPPER_DIR is the home dir */
					printf("for shared dir,its upper is home dir\n");
					lkup_node = home_dir;
				}else{
					printf("go to parent\n");
					/* just go back to the parent */
					lkup_node = lkup_node->parent;
				}
				/* set ret to tell caller this is a UPPER_DIR */
				serrmsg("IS UPPER_DIR \n");
				ret = 2;
				goto cont;
			}
	/************************************** NO SUCH FILE OR DIR *******************************************/
	/**/	if(lkup_node->how_many_children == 0){
	/**/		/* no children */
	/**/		serrmsg("NO CHILD IN LKUPNODE");
	/**/		printf("no child in dir -- %s\n",lkup_node->name);
	/**/		i = 0;
	/**/		ret = 3;
	/**/		goto cont;
	/**/	}
	/**/	i = binary_seach_file(file_name_buf,lkup_node->child,0,lkup_node->how_many_children - 1);
	/**/	if(strcmp(file_name_buf,lkup_node->child[i]->name) != 0){
	/**/		/* look up fail */
	/**/		printf("no such file or dir -- %s\n",file_name_buf);
	/**/		serrmsg("%s NO SUCH FILE OR DIRECTORY UNDER DIRECTORY %s",file_name_buf,lkup_node->name);
	/**/		ret = 3;
	/**/		/* just continue to see if still there are something in the file_path */
	/**/		goto cont;
	/**/	}
	/*******************************************************************************************************/
			/* go to next dir */
			lkup_node = lkup_node->child[i];
cont:
			/* reset lookup */
			if(ret != 3){
				if(IS_DIR_FILE(lkup_node->file_type) && IS_SHARED_FLAG_SET(lkup_node->file_type)){
					/* this is the user's shared dir */
					lkup_node = shared_dir;
				}
				fn_head = NULL;
				fn_tail = NULL;
			}
		}
	}
op_over:
	if(index != NULL){
		*index = i;
	}
	if(nsnode != (ns_node **)0){
		*nsnode = lkup_node;
	}
	return ret;
}
/*-------------------------------------split------------------------------------*/
ns_node * mkfile(u8 * file_path,u8 file_type,u32 acl)
{
	/* make a new file
	 * ONLY REGULAR FILE AND DIRECTORY FILE
	 * NO SHARED_FLAG OR SHARED_DIR IS ALLOWED HERE!
	 * 1) THE /user/shared DIR WILL BE CREATED WITH SHARED_FLAG SET WHEN A NEW USER IS ADDED TO CFS
	 * 2) /shared DIR WILL BE CREATED WITH SHARED_DIR SET WHEN INITIALIZE SYSTEM */
	u32 index;
	u32 r,j;
	ns_node * nsnode;
	ns_node * new_file = (ns_node *)0;
	ns_node ** child;
	u8 file_name_buf[FILE_PATH_LEN];
	if(!IS_LEGAL_FILE_TYPE(file_type)){
		serrmsg("ILLEGAL FILE TYPE!");
		goto ret;
	}
	bzero(file_name_buf,FILE_PATH_LEN);
	r = get_ns_node(file_path,&nsnode,&index,file_name_buf,FILE_PATH_LEN);
	printf("MKFIEL : RETURN VALUE FROM GET_NS_NODE  -- %d\n",r);
	if(r == 3){
		printf("new file -- %s will be created under dir %s\n",file_name_buf,nsnode->name);
		/* ready to make a new file! */
		if(strlen(file_name_buf) == 0 || nsnode == (ns_node *)0){
			/* some error happened */
			serrmsg("NEW FILE_NAME OR ITS PARENT NODE INVLAID!");
			goto ret;
		}
		printf("ready to make a new file : %s \n \t\t under dir : %s\n",file_name_buf,nsnode->name);
		child = nsnode->child;
		new_file = (ns_node *)calloc(1,sizeof(ns_node));
		if(new_file == (ns_node *)0){
			serrmsg("CALLOC FAIL!");
			goto ret;
		}
		j = strlen(file_name_buf);
		new_file->name = calloc(1,j + 1);
		if(new_file->name == NULL){
			free(new_file);
			serrmsg("CALLOC FAIL!");
			new_file = (ns_node *)0;
			goto ret;
		}
		strncpy(new_file->name,file_name_buf,j);
		new_file->file_type = file_type;
		new_file->uid = current_user.uid;
		new_file->gid = current_user.gid;
		new_file->acl = acl;
		new_file->parent = nsnode;
		new_file->child = (ns_node **)0;
		new_file->how_many_children = 0;
		child = (ns_node **)realloc(nsnode->child,(++(nsnode->how_many_children)) * sizeof(ns_node *));
		if(child == (ns_node **)0){
			free(new_file->name);
			free(new_file);
			serrmsg("REALLOC FAIL!");
			new_file = (ns_node *)0;
			goto ret;
		}
		nsnode->child = child;
		for(j = nsnode->how_many_children - 1;j > index;j--){
			child[j] = child[j-1];
		}
		child[index] = new_file;
	}
ret:
	return new_file;
}
/*-------------------------------------split------------------------------------*/
u32 rmfile(u8 * file_path)
{
	/* remove file "file_path" */
	ns_node * rmnode,*parent_dir;
	ns_node ** child;
	u32 i,j,k,r;
	u8 file_name_buf[FILE_PATH_LEN];
	bzero(file_name_buf,FILE_PATH_LEN);
	r = get_ns_node(file_path,&rmnode,&i,file_name_buf,FILE_PATH_LEN);
	if(r == 0){
		/* ready to delete file */
		parent_dir = rmnode->parent;
		child = parent_dir->child;
		parent_dir->child = (ns_node **)calloc(--(parent_dir->how_many_children),sizeof(ns_node *));
		for(j = 0,k=0;j < parent_dir->how_many_children;j++,k++){
			if(k == i){k++;}
			parent_dir->child[j] = child[k];
		}
		free(child);
		/*----------------------------------------------------------*/
		/*															*/
		/*															*/
		/*        call posix function to remove file here			*/
		/*															*/
		/*															*/
		/*----------------------------------------------------------*/
		free(rmnode->name);
		free(rmnode);
	}else{return 1;}/* get_ns_node fail */
	return 0;
}
/*-------------------------------------split------------------------------------*/
u32 cd(u8 * file_path)
{
	/* get_ns_node return 0 or 2 */
	u32 r,i,ret = 0;
	u8 file_name_buf[FILE_PATH_LEN];
	ns_node * dest_dir;
	bzero(file_name_buf,FILE_PATH_LEN);
	r = get_ns_node(file_path,&dest_dir,&i,file_name_buf,FILE_PATH_LEN);
	switch(r){
		case 0:
			if(dest_dir->file_type != DIRECTORY_FILE){
				ret = 1;
			}else{
				current_working_dir = dest_dir;
			}
			break;
		case 2:
			current_working_dir = dest_dir;
			break;
		default:
			/* cd error */
			break;
	}
	return ret;
}
/*-------------------------------------split------------------------------------*/
u32 ls(u8 * file_path,ns_node ** single_file,ns_node *** all_file_in_dir,u32 * how_many_children)
{
	/* list file
	 * return 0 on fail! */
	u32 r,i,ret = 0;
	u8 file_name_buf[FILE_PATH_LEN];
	ns_node * lsfile;
	bzero(file_name_buf,FILE_PATH_LEN);
	r = get_ns_node(file_path,&lsfile,&i,file_name_buf,FILE_PATH_LEN);
	if(r == 0 || r == 2){
		if(IS_REG_FILE(lsfile->file_type)){
			*single_file = lsfile;
			ret = 1;
		}else if(IS_DIR_FILE(lsfile->file_type)){
			*all_file_in_dir = lsfile->child;
			*how_many_children = lsfile->how_many_children;
			ret = 2;
		}
	}
	return ret;
}
/*-------------------------------------split------------------------------------*/
u32 get_user_info_by_name(u8 * user_name,u8 * buf,u32 bufsiz)
{
	/* get user info by name
	 * RETURN VALUE : 0 on success,user info will be in buf
	 *				  > 0 fail */
	FILE *fp;
	u32 ret = 0;
	u32 i = 0;
	bzero(buf,bufsiz);
	snprintf(buf,bufsiz,GET_USER_INFO_CMD_FMT,user_name,USER_INFO_FILE);
	printf("get_user_info_by_name command_line : %s\n",buf);
	if((fp = popen(buf,"r")) == NULL){
		fprintf(stderr,"popen fail!\n");
		ret = 1;
		goto op_over;
	}
	bzero(buf,bufsiz);
	while(fgets(buf + i,bufsiz - i,fp) != NULL){i = strlen(buf);}
	pclose(fp);
	if(strlen(buf) == 0){
		ret = 2;
	}
op_over:
	return ret;
}
/*-------------------------------------split------------------------------------*/
u32 adduser(u8 * user_name,u8 * pw)
{
	/* add a new individual user in cfs
	 * 1) get a uid
	 * 2) make home dir
	 * 3) update user_info.txt */
	ns_node * hd,*user_shared_dir;
	FILE *fp;
	u32 ret = 0;
	u32 uid,gid,i = 0;
	u8 buf[USER_INFO_BUFSZ];
	bzero(buf,USER_INFO_BUFSZ);
	/* check if user name already exist */
	if(get_user_info_by_name(user_name,buf,USER_INFO_BUFSZ) != 2){
		/* user already exist or popen error */
		ret = 1;
		goto op_over;
	}
	/* user name is ok!
	 * now get the least available uid for new user */
	if((fp = popen(GET_LEAST_AVAILABLE_UID,"r")) == NULL){
		ret = 2;
		goto op_over;
	}
	while(fgets(buf + i,USER_INFO_BUFSZ - i,fp) != NULL){i = strlen(buf);}
	pclose(fp);
	uid = atoi_u32(buf);
	gid = uid;
	bzero(buf,USER_INFO_BUFSZ);
	snprintf(buf,USER_INFO_BUFSZ,USER_INFO_FMT,uid,gid,user_name,user_name,pw);
	/* write user information to file */
	if((i = open(USER_INFO_FILE,O_WRONLY)) < 0){
		ret = 3;
		goto op_over;
	}
	lseek(i,0,SEEK_END);
	if(write(i,buf,strlen(buf)) < 0){
		close(i);
		ret = 4;
		goto op_over;
	}
	close(i);
	/* create home dir for new user */
	set_cu(uid,gid);
	bzero(buf,USER_INFO_BUFSZ);
	snprintf(buf,USER_INFO_BUFSZ,"/%s",user_name);
	hd = mkfile(buf,DIRECTORY_FILE,0760);
	/* make shared dir in user's home dir */
	bzero(buf,USER_INFO_BUFSZ);
	snprintf(buf,USER_INFO_BUFSZ,"/%s/%s",user_name,SHARED_DIR_NAME);
	user_shared_dir = mkfile(buf,DIRECTORY_FILE,0760);
	user_shared_dir->file_type |= SHARED_FLAG;
	/* set current user back to super user
	 * coz only super user has the right to add a new user */
	set_cu(SU_UID,SU_GID);
op_over:
	return ret;
}
/*-------------------------------------split------------------------------------*/
u32 user_login()
{
	/* set something
	 * 1) read user_name&pw from stdin for authentication
	 * 2) set current user
	 * 3) set current working dir
	 * 4) set home dir
	 * */
	u32 ret = 0;
	u8 * p,*q;
	u8 * user_name_hint = "user : ";
	u8 * pw_hint = "password : ";
	u8 user_input_buf[USER_INFO_BUFSZ];
	u32 i,j,r,index;
	cfs_user login_user;
	ns_node * login_home_dir;
	u8 user_info_buf[USER_INFO_BUFSZ];
	bzero(user_input_buf,USER_INFO_BUFSZ);
	fputs(user_name_hint,stdout);
	fgets(user_input_buf,USER_INFO_BUFSZ,stdin);
	i = strlen(user_input_buf);
	user_input_buf[i-1] = '\0';
	r = get_user_info_by_name(user_input_buf,user_info_buf,USER_INFO_BUFSZ);
	if(r != 0){
		ret = 1;
		fprintf(stderr,"user not exist!\n");
		goto op_over;
	}
	i = strlen(user_info_buf);
	user_info_buf[--i] = '\0';
	printf("user info : %s\n",user_info_buf);
	/* now user's info are in user_info_buf */
	bzero(user_input_buf,USER_INFO_BUFSZ);
	fputs(pw_hint,stdout);
	fgets(user_input_buf,USER_INFO_BUFSZ,stdin);
	j = strlen(user_input_buf);
	user_input_buf[--j] = '\0';
	if(*(user_info_buf + i - j - 1) == CH_SPACE && strncmp(user_input_buf,user_info_buf + i - j,j) == 0){
		/* authentication ok */
		p = user_info_buf;
		q = p;
		while(*p != CH_SPACE){p++;}
		*p = '\0';
		login_user.uid = atoi_u32(q);
		q = p + 1;
		while(*q == CH_SPACE){q++;}
		p = q;
		while(*p != CH_SPACE){p++;}
		*p = '\0';
		login_user.gid = atoi_u32(q);
		q = p + 1;
		while(*q == CH_SPACE){q++;}
		p = q;
		while(*p != CH_SPACE){p++;}
		*p = '\0';
		bzero(user_input_buf,USER_INFO_BUFSZ);
		r = get_ns_node(q,&login_home_dir,&index,user_input_buf,USER_INFO_BUFSZ);
		if( r != 0 && r != 2){
			ret = 2;
			perrmsg("get_home_dir");
			goto op_over;
		}
		set_cwd(login_home_dir);
		set_cu(login_user.uid,login_user.gid);
		home_dir = login_home_dir;
	}else{
		fprintf(stderr,"INCORRECT PW!\n");
		ret = 3;
		goto op_over;
	}
op_over:
	return ret;
}
/*-------------------------------------split------------------------------------*/
void cfs_shell()
{
	/* called after user login */
}
/*-------------------------------------split------------------------------------*/
void print_ns_node(ns_node * node)
{
	u8 * file_type;
	if(IS_DIR_FILE(node->file_type)){
		file_type = "DIRECTORY_FILE";
	}else if(IS_REG_FILE(node->file_type)){
		file_type = "REGULAR_FILE";
	}
	printf("file_type : %s\n",file_type);
	printf("file_name : %s\n",node->name);
	printf("uid		  : %d\n",node->uid);
	printf("gid		  : %d\n",node->gid);
	printf("acl		  : %o\n",node->acl);
	printf("parent	  : %s\n",node->parent->name);
	printf("child_num : %d\n",node->how_many_children);
	return;
}
/*-------------------------------------split------------------------------------*/
static rbt * get_node_by_key(u32 key,u32 * di)
{
	rbt * node = root;
	if(node == NIL_NODE){
		*di = 0;
		return NIL_NODE;
	}
	while(1){
		if(key < i(node)){
			if(l(node) == NIL_NODE){
				*di = -1;
				break;
			}else{
				node = l(node);
			}
		}else if(key > i(node)){
			if(r(node) == NIL_NODE){
				*di = 1;
				break;
			}else{
				node = r(node);
			}
		}else{
			*di = 0;
			break;
		}
	}
	return node;
}
static void left_rotate(rbt * x)
{
	rbt * y = r(x);
	p(y) = p(x);
	r(x) = l(y);
	if(l(y) != NIL_NODE){
		p(l(y)) = x;
	}
	if(p(x) == NIL_NODE){
		root = y;
	}else{
		if(l(p(x)) == x){
			l(p(x)) = y;
		}else if(r(p(x)) == x){
			r(p(x)) = y;
		}
	}
	l(y) = x;
	p(x) = y;
	return;
}
static void right_rotate(rbt * x)
{
	rbt * y = l(x);
	p(y) = p(x);
	l(x) = r(y);
	if(r(y) != NIL_NODE){
		p(r(y)) = x;
	}
	if(p(x) == NIL_NODE){
		root = y;
	}else{
		if(l(p(x)) == x){
			l(p(x)) = y;
		}else if(r(p(x)) == x){
			r(p(x)) = y;
		}
	}
	r(y) = x;
	p(x) = y;
	return;
}
static rbt * rb_max(rbt * z)
{
	while(r(z) != NIL_NODE){z = r(z);}
	return z;
}
static rbt * rb_min(rbt * z)
{
	while(l(z) != NIL_NODE){z = l(z);}
	return z;
}
static rbt * rb_suc(rbt * z)
{
	rbt * x;
	if(z == NIL_NODE){
		fprintf(stderr,"NO SUC_NODE FOR NIL_NODE\n");
		return z;
	}
	if(r(z) != NIL_NODE){
		return rb_min(r(z));
	}
	x = z;
	while( p(x) != NIL_NODE && x == r(p(x))){
		x = p(x);
	}
	return p(x);
}
static rbt * rb_pre(rbt * z)
{
	rbt * x;
	if(z == NIL_NODE){
		fprintf(stderr,"NO PRE_NODE FOR NIL_NODE\n");
		return z;
	}
	if(l(z) != NIL_NODE){
		return rb_max(l(z));
	}
	x = z;
	while(p(x) != NIL_NODE && x == l(p(x))){
		x = p(x);
	}
	return p(x);
}
static rbt * new_rbt_node(u32 key)
{
	rbt * new_node = (rbt *)malloc(sizeof(rbt));
	if(new_node == NULL){
		return new_node;
	}
	i(new_node) = key;
	l(new_node) = NIL_NODE;
	r(new_node) = NIL_NODE;
	p(new_node) = NIL_NODE;
	c(new_node) = RBT_RED;
	return new_node;
}
static void print_rbt_node(rbt * node,u8 i)
{
	u8 * lpr;
	if(node == NIL_NODE){
		fprintf(stderr,"CANNOT PRINT A NULL NODE!\n");
		return;
	}
	switch(i){
		case LEFT_CHILD:
			lpr = H_LEFT_CHILD;
			break;
		case PARENT:
			lpr = H_PARENT;
			break;
		case RIGHT_CHILD:
			lpr = H_RIGHT_CHILD;
			break;
		default:
			fprintf(stderr,"UNRECOGNIZED I\n");
			return;
	}
	printf("%s%3d -- %c\n",lpr,i(node),c(node));
	return;
}
static void print_rbt(rbt * r)
{
	rbt * n = r;
	u8 * s;
	if(n == NIL_NODE){
		return;
	}
	print_rbt_node(n,PARENT);
	if(l(n) != NIL_NODE){
		print_rbt_node(l(n),LEFT_CHILD);
	}
	if(r(n) != NIL_NODE){
		print_rbt_node(r(n),RIGHT_CHILD);
	}
	printf("------------------------------------\n");
	print_rbt(l(n));
	print_rbt(r(n));
	return;
}
static void rb_insert_fixup(rbt * z)
{
	rbt * y;
	while(c(p(z)) == RBT_RED){
		printf("current z_value is -- %d\n",i(z));
		/* p(z) not null and color is red,
		 * so p(z) is not root --> p(p(z)) exists */
		if(p(z) == l(p(p(z)))){
			/* p(z) is left child of its parent */
			y = r(p(p(z)));
			/*------------- case 1 --------------*/
			if(c(y) == RBT_RED){
				/* p(z) 's parent must be black! */
				c(y) = RBT_BLACK;
				c(p(z)) = RBT_BLACK;
				c(p(p(z))) = RBT_RED;
				z = p(p(z));
			/*------------- case 2 --------------*/
			}else if(z == r(p(z))){
				/* y == NIL_NODE or c(y) is black */
				z = p(z);
				left_rotate(z);
			/*------------- case 3 --------------*/
			}else{
				/* z is left child of its parent */
				c(p(z)) = RBT_BLACK;
				c(p(p(z))) = RBT_RED;
				right_rotate(p(p(z)));
			}
		}else{
			/* p(z) is right child of its parent */
			y = l(p(p(z)));
			if(c(y) == RBT_RED){
				c(y) = RBT_BLACK;
				c(p(z)) = RBT_BLACK;
				c(p(p(z))) = RBT_RED;
				z = p(p(z));
			}else if(z == l(p(z))){
				z = p(z);
				right_rotate(z);
			}else{
				c(p(z)) = RBT_BLACK;
				c(p(p(z))) = RBT_RED;
				left_rotate(p(p(z)));
			}
		}
	}
	c(root) = RBT_BLACK;
	return;
}
static void rb_insert(rbt * z)
{
	u32 di;
	rbt * node = get_node_by_key(i(z),&di);
	printf("rb_insert : get_node_by_key -- di : %d\n",di);
	p(z) = node;
	if(node == NIL_NODE){
		/* empty rb_tree */
		printf("rb_insert : first node will be inserted!\n");
		root = z;
	}else if(di == 0){
		/* node with key==z->i already exists */
		printf("key already exists!\n");
		return;
	}else{
		if(di == -1){
			l(node) = z;
		}else if(di == 1){
			r(node) = z;
		}
	}
	l(z) = NIL_NODE;
	r(z) = NIL_NODE;
	c(z) = RBT_RED;
	rb_insert_fixup(z);
	return;
}
static void rb_delete_fixup(rbt * z)
{
	rbt * x = z,*w;
	while(x != root && c(x) == RBT_BLACK){
		if(x == l(p(x))){
			w = r(p(x));
			/*-------- case 1 --------*/
			if(c(w) == RBT_RED){
				/* brother of x is red,
				 * so c(p(x)) is black,
				 * repaint and rotate to give x a brother whose color is black */
				c(w) = RBT_BLACK;
				c(p(x)) = RBT_RED;
				left_rotate(p(x));
				w = r(p(x));
			/*-------- case 2 --------*/
			}else if(c(l(w)) == RBT_BLACK && c(r(w)) == RBT_BLACK){
				/* color of w is black,
				 * and two children are all black.
				 * ready to move black color from x&w to their parent.*/
				c(w) = RBT_RED;
				x = p(x);
			/*-------- case 3 --------*/
			}else if(c(r(w)) == RBT_BLACK){
				/* w is black,
				 * right child of w is black,
				 * left child is red. */
				c(l(w)) = RBT_BLACK;
				c(w) = RBT_RED;
				right_rotate(w);
				w = r(p(x));
			/*-------- case 4 --------*/
			}else{
				/* right child of w is red,
				 * w is black. */
				c(w) = c(p(x));
				c(p(x)) = RBT_BLACK;
				c(r(w)) = RBT_BLACK;
				left_rotate(p(x));
				x = root;
			}
		}else if(x == r(p(x))){
			w = l(p(x));
			if(c(w) == RBT_RED){
				c(w) = RBT_BLACK;
				c(p(x)) = RBT_RED;
				right_rotate(p(x));
				w = l(p(x));
			}else if(c(l(w)) == RBT_BLACK && c(r(w)) == RBT_BLACK){
				c(w) = RBT_RED;
				x = p(x);
			}else if(c(l(w)) == RBT_BLACK){
				/* right child of w is red */
				c(w) = RBT_RED;
				c(r(w)) = RBT_BLACK;
				left_rotate(w);
				w = l(p(x));
			}else{
				/* left child of w is red */
				c(w) = c(p(x));
				c(l(w)) = RBT_BLACK;
				c(p(x)) = RBT_BLACK;
				right_rotate(p(x));
				x = root;
			}
		}
	}
	c(x) = RBT_BLACK;
	return;
}
static rbt * rb_delete(u32 key)
{
	u32 di;
	rbt *x,*y;
	rbt * z = get_node_by_key(key,&di);
	if(z == NIL_NODE || di != 0){
		fprintf(stderr,"NO NODE WITH KEY %d\n",key);
		return NIL_NODE;
	}
	/* z != NIL_NODE && di == 0 */
	/* y is the node to be deleted */
	if(l(z) == NIL_NODE || r(z) == NIL_NODE){
		y = z;
	}else{
		y = rb_suc(z);
	}
	if(l(y) != NIL_NODE){
		x = l(y);
	}else{
		x = r(y);
	}
	p(x) = p(y);
	if(p(y) == NIL_NODE){
		root = x;
	}else{
		if(y == l(p(y))){
			l(p(y)) = x;
		}else if(y == r(p(y))){
			r(p(y)) = x;
		}
	}
	if(y != z){
		i(z) = i(y);
	}
	if(c(y) == RBT_BLACK){
		rb_delete_fixup(x);
	}
	return y;
}
static void init_rbt()
{
	rbt_nil_node.c = RBT_BLACK;
	NIL_NODE = &rbt_nil_node;
	root = NIL_NODE;
}
/* namespace initialization */
static void init_ns()
{
	/* dirs need to be initialized
	 * 1) /
	 * 2) super user
	 * 3) /shared/
	 * */
	/* root dir */
	u32 fd;
	u8 buf[USER_INFO_BUFSZ];
	ns_node * newnode;
	printf("init cfs root dir...\n");
	cfs_root_dir.name = ROOT_DIR_NAME;
	cfs_root_dir.file_type = DIRECTORY_FILE;
	cfs_root_dir.uid = SU_UID;
	cfs_root_dir.gid = SU_GID;
	cfs_root_dir.acl = 0700;
	cfs_root_dir.parent = &cfs_root_dir;/* parent dir for root is itself */
	cfs_root_dir.child = (struct NS_NODE **)0;
	cfs_root_dir.how_many_children = 0;
	print_ns_node(&cfs_root_dir);
	printf("init cfs super user...\n");
	/* super user */
	super_user.uid = SU_UID;
	super_user.gid = SU_GID;
	bzero(buf,USER_INFO_BUFSZ);
	snprintf(buf,USER_INFO_BUFSZ,USER_INFO_FMT,SU_UID,SU_GID,SU_NAME,SU_NAME,SU_INIT_PW);
	printf("super user info :\n %s\n",buf);
	if((fd = open(USER_INFO_FILE,O_WRONLY)) < 0){
		return;
	}
	lseek(fd,0,SEEK_END);
	write(fd,buf,strlen(buf));
	close(fd);
	set_cu(SU_UID,SU_GID);
	set_cwd(&cfs_root_dir);
	/* super user's information write to file */
	printf("make home dir for super user...\n");
	if((newnode = mkfile(HOME_FOR_SU,DIRECTORY_FILE,0700)) == (ns_node *)0){
		perrmsg("make home from su");
		return;
	}
	print_ns_node(newnode);
	printf("make shared dir...\n");
	if((newnode = mkfile(HOME_FOR_SHARED,DIRECTORY_FILE,0777)) == (ns_node *)0){
		perrmsg("make shared dir");
		return;
	}
	printf("set SHARED_DIR flag...\n");
	newnode->file_type = newnode->file_type | SHARED_DIR;
	print_ns_node(newnode);
	return;
}
