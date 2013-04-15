#ifndef OBJECT_RESPONSE_HEADER_H
#define OBJECT_RESPONSE_HEADER_H

#include <sys/stat.h>
#include <stdio.h>
#include <microhttpd.h>
#include "container.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#define BUFLENGTH 128


int get_object_header_yes(struct MHD_Response *response,struct sender_head send_header);
int get_object_header_no(struct MHD_Response *response,struct sender_head send_header);
int head_object_header_yes(struct MHD_Response *response,struct sender_head send_header);
int head_object_header_no(struct MHD_Response *response,struct sender_head send_header);
int post_object_header(struct MHD_Response *response,struct sender_head send_header);
int delete_object_header_yes(struct MHD_Response *response,struct sender_head send_header);
int delete_bucket_header_yes(struct MHD_Response *response,struct sender_head send_header);
int head_bucket_header_yes(struct MHD_Response *response,struct sender_head send_header);
int put_bucket_header_yes(struct MHD_Response *response,struct sender_head send_header);
int get_bucket_header_yes(struct MHD_Response *response,struct sender_head send_header);
int get_bucket_header_no(struct MHD_Response *response,struct sender_head send_header);
int head_bucket_header_no(struct MHD_Response *response,struct sender_head send_header);


int Gen_Date(char *date);
int Tran_Date(time_t time,char *date);


int err_sys(const char *info);


//int iterate_post (void *coninfo_cls, enum MHD_ValueKind kind, const char *key,const char *filename, const char *content_type,const char *transfer_encoding, const char *data, uint64_t off,size_t size);
int iterate_put (void *coninfo_cls, enum MHD_ValueKind kind, const char *key,const char *filename, const char *content_type,const char *transfer_encoding, const char *data, uint64_t off,size_t size);
void put_request_completed (void *cls, struct MHD_Connection *connection,void **con_cls, enum MHD_RequestTerminationCode toe);
int post_send_page (struct MHD_Connection *connection, const char *page,int status_code);
//int send_page(struct MHD_Connection *connection, const char *page,int status_code);

//int iterate_post(void *coninfo_cls, enum MHD_ValueKind kind, const char *key,const char *filename, const char *content_type,const char *transfer_encoding, const char *data, uint64_t off,size_t size);
//void request_completed (void *cls, struct MHD_Connection *connection,void **con_cls, enum MHD_RequestTerminationCode toe);
void post_request_completed (void *cls, struct MHD_Connection *connection,void **con_cls, enum MHD_RequestTerminationCode toe);
int put_send_page (struct MHD_Connection *connection, const char *page,int status_code);

//***************************************************************************************
//***************************************************************************************

 void
request_completed (void *cls, struct MHD_Connection *connection,
                   void **con_cls, enum MHD_RequestTerminationCode toe);



int
iterate_post (void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
              const char *filename, const char *content_type,
              const char *transfer_encoding, const char *data, uint64_t off,
              size_t size);


 int  send_page (struct MHD_Connection *connection, const char *page,int status_code);


#endif































