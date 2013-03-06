link_lib = -lpthread
objs_main = modify_md.o md_mutex_server_process.o lock_file.o try_lock_file.o unlock_file.o new_md_mutex_server.o
objs = md_mutex_client_func.o hash.o
.PHONY : all md_mutex_server lock unlock try_lock modify_md new_md_mutex_server
all : md_mutex_server lock unlock try_lock modify_md new_md_mutex_server
new_md_mutex_server : new_md_mutex_server.o
	gcc new_md_mutex_server.o hash.o -g -o new_md_mutex_server $(link_lib)
md_mutex_server : md_mutex_server_process.o
	gcc md_mutex_server_process.o -g -o md_mutex_server $(link_lib)
lock: lock_file.o $(objs)
	gcc lock_file.o $(objs) -g -o lock
try_lock : try_lock_file.o $(objs)
	gcc try_lock_file.o $(objs) -g -o try_lock
unlock : unlock_file.o $(objs)
	gcc unlock_file.o $(objs) -g -o unlock
modify_md : modify_md.o $(objs)
	gcc modify_md.o $(objs) -g -o modify_md
.PHONY : clean
clean : 
	rm $(objs_main) $(objs) md_mutex_server lock try_lock unlock modify_md new_md_mutex_server
