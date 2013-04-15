#ifndef REQUEST_ANALYSIS_H
#define REQUEST_ANALYSIS_H

#include <microhttpd.h>
#include "container.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include "xml.h"

#define POSTBUFFERSIZE 512
#define BUFTESTSIZE 4096
#define POST 1
#define STDOUT_FIFENO 0
#define MAXCLIENTS      2

#define GET             0
//#define POST            1


int
request_get(void *cls, struct MHD_Connection *connection,         \
                      const char *url, const char *method,         \
                      const char *version,const char *upload_data,   \
                      size_t *upload_data_size, void **con_cls);

int
request_put(void *cls, struct MHD_Connection *connection,         \
                      const char *url, const char *method,         \
                      const char *version,const char *upload_data,   \
                      size_t *upload_data_size, void **con_cls);

int
request_delete(void *cls, struct MHD_Connection *connection,         \
                      const char *url, const char *method,         \
                      const char *version,const char *upload_data,   \
                      size_t *upload_data_size, void **con_cls);

int
request_post(void *cls, struct MHD_Connection *connection,         \
                      const char *url, const char *method,         \
                      const char *version,const char *upload_data,   \
                      size_t *upload_data_size, void **con_cls);


int
request_head(void *cls, struct MHD_Connection *connection,         \
                      const char *url, const char *method,         \
                      const char *version,const char *upload_data,   \
                      size_t *upload_data_size, void **con_cls);

static unsigned int nr_of_uploading_clients = 0;

struct connection_info_struct
{
int connectiontype;
struct MHD_PostProcessor *postprocessor;
FILE *fp;
const char *answerstring;
int answercode;
};

static const char *askpage = "<html><body>\n\
                       Upload a file, please!<br>\n\
                       There are %u clients uploading at the moment.<br>\n\
                       <form action=\"/filepost\" method=\"post\" enctype=\"multipart/form-data\">\n\
                       <input name=\"file\" type=\"file\">\n\
                       <input type=\"submit\" value=\" Send \"></form>\n\
                       </body></html>";

static const char *busypage =
  "<html><body>This server is busy, please try again later.</body></html>";

static const char *completepage =
  "<html><body>The upload has been completed.</body></html>";

static const char *errorpage =
  "<html><body>This doesn't seem to be right.</body></html>";
static const char *servererrorpage =
  "<html><body>An internal server error has occured.</body></html>";
static const char *fileexistspage =
  "<html><body>This file already exists.</body></html>";



#endif

