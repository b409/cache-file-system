#ifndef XML_H
#define XML_H
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>
#include <libxml/parser.h>
#include <libxml/tree.h>     
#include <string.h>
#include <time.h>
#include <libxml/xmlmemory.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <stdbool.h>


//xmlDocPtr parseDoc(char *docname, char *keyword) ;
//extern 
int creat_xml(char *x_file);
//extern 
void add_count (char*DocName, struct stat *buf,char *filename);
void delete_file_xml(const char *path,int *count,char *xmlname);
bool is_dir_xml(const char *path);
char* my_itoa(int val);
int get_sonstr(char *path,char *filename);


#endif

