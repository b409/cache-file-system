link_lib = -L/usr/local/lib -ltokyotyrant -ltokyocabinet -lz -lbz2 -lresolv -lnsl -ldl -lrt -lpthread -lm -lc
objs = tt_pgo.o
obj_main = tt_get.o tt_put.o tt_out.o  iod_test.o
.PHONY : all tt_get tt_out tt_put iod_test
all : tt_get tt_out tt_put iod_test
tt_get : tt_get.o tt_pgo.o
	gcc tt_get.o tt_pgo.o -g -o tt_get $(link_lib)
tt_out : tt_out.o tt_pgo.o
	gcc tt_out.o tt_pgo.o -g -o tt_out $(link_lib)
tt_put : tt_put.o tt_pgo.o
	gcc tt_put.o tt_pgo.o -g -o tt_put $(link_lib)
iod_test : iod_test.o tt_pgo.o
	gcc iod_test.o tt_pgo.o -g -o iod_test $(link_lib)
.PHONY : clean
clean :
		rm $(objs) $(obj_main) tt_get tt_put tt_out iod_test

