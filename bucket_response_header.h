#ifndef BUCKET_RESPONSE_HEADER_H
#define BUCKET_RESPONSE_HEADER_H



#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include "container.h"

int get_bucket_header_yes(struct MHD_Response *response,struct sender_head send_header);

int get_bucket_header_no(struct MHD_Response *response,struct sender_head send_header);

#endif
