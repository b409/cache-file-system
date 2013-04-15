#s3:contain.o request_analysis.o object_response_header.o bucket_response_header.o delete_bucket.o xml.o
#source file
SOURCE=contain.c contain.h request_analysis.c request_analysis.h object_response_header.c object_response_header.h bucket_response_header.c bucket_response_header.h delete_bucket.c delete_bucket.h xml.c xml.h 

#which compiler
CC=gcc

CFLAGS=-w -g
INCLUDE = -I/usr/local/include/libxml2 -I/usr/local/include

LINK_LIB = -L/usr/local/lib -ltokyotyrant -ltokyocabinet -lbz2 -lresolv -lnsl -lc -ldl -lrt -lxml2 -lz -lm -lpthread -lsctp -lmicrohttpd

OBJECTS= object_response_header.o request_analysis.o  bucket_response_header.o delete_bucket.o xml.o container.o utility.o xml_msg.o io_queue.o tt_func.o sn_sckt.o posix_for_s3.o

#s3:contain.o request_analysis.o object_response_header.o bucket_response_header.o delete_bucket.o xml.o
all:s3
s3:$(OBJECTS)
	#$(CC) $(CFLAGS) $(INCLUDE) $(LINK_LIB) $(OBJECTS) -o s3
	$(CC) $(CFLAGS) $(INCLUDE) $(LINK_LIN) -o s3 $(OBJECTS)
#	$(CC) $(CFLAGS) -I$(INCLUDEX) -I$(INCLUDEH) $(LIB) -o s3 $^
%.o:%.c
#	%.o
	$(CC) $(CFLAGS) -I$(INCLUDEX) -I$(INCLUDEH) $(LIB) -c $<

.PHONY:clean
clean:
	-rm $(OBJECTS) s3





