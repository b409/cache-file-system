# changed by Jin (2013-03-05)
# <change> CFLAGS /usr/local/include/libxml2
# <add> io_queue.o in OBJS
# <add> io_queue_test.o in OBJS_M

CC_OP = -g -o
CFLAGS = -I/usr/local/include/libxml2
LINK_LIB = -L/usr/local/lib -ltokyotyrant -ltokyocabinet -lbz2 -lresolv -lnsl -lc -ldl -lrt -lxml2 -lz -lm -lpthread -lsctp 
OBJS_M = sn_main.o  make_xml.o parse_xml.o io_queue_test.o
OBJS =  utility.o xml_msg.o io_queue.o tt_func.o sn_sckt.o

ALL = sn_main  mk_xml_to_stdout parse_xml_from_stdin io_queue_test
.PHONY : all sn_main  mk_xml_to_stdout parse_xml_from_stdin io_queue_test
all : sn_main  xml_msg.o mk_xml_to_stdout parse_xml_from_stdin io_queue_test

sn_main : sn_main.o xml_msg.o utility.o
	gcc sn_main.o xml_msg.o utility.o $(CC_OP) sn_main $(LINK_LIB)
io_queue_test : io_queue_test.o io_queue.o tt_func.o sn_sckt.o xml_msg.o utility.o
	gcc io_queue_test.o io_queue.o tt_func.o sn_sckt.o xml_msg.o utility.o $(CC_OP) io_queue_test $(LINK_LIB)
mk_xml_to_stdout : make_xml.o
	gcc make_xml.o $(CC_OP) mk_xml_to_stdout $(LINK_LIB)
parse_xml_from_stdin : parse_xml.o xml_msg.o utility.o
	gcc parse_xml.o xml_msg.o utility.o $(CC_OP) parse_xml_from_stdin $(LINK_LIB)
xml_msg.o :
	gcc $(CFLAGS) -c xml_msg.c
.PHONY : clean
clean : 
	rm $(OBJS_M) $(OBJS) $(ALL)
