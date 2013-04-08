/* cache file system namespace manager
 * BY grant chen 27,Feb,2013 */
#include"glob.h"
#include"nss.h"

static rbt rbt_nil_node;
static rbt * NIL_NODE;
static rbt * root;

/* initialized  on login */
static cfs_user current_user;  /* current user */
static ns_node * current_working_dir; /* current working dir */
static ns_node * home_dir;      /* user's home dir,set when user login */
/* initialized on init_ns */
static ns_node cfs_root_dir; /* ns_node of root dir*/
static ns_node * shared_dir;    /* shared dir */
static cfs_user super_user;
static cfs_user * active_user_list; /* all active user in cfs are in a list */
int main()
{
	init_ns();
	if(user_login() == 0){
		/* login success! */
		printf("login ok!\n");
	}
	return 0;
}
