#include "xml.h"


bool is_dir_xml(const char *path)
{
    struct stat statbuf;
    if(lstat(path, &statbuf) ==0)//lstat·µ»ØÎÄŒþµÄÐÅÏ¢£¬ÎÄŒþÐÅÏ¢Žæ·ÅÔÚstatœá¹¹ÖÐ
    {
        return S_ISDIR(statbuf.st_mode) != 0;//S_ISDIRºê£¬ÅÐ¶ÏÎÄŒþÀàÐÍÊÇ·ñÎªÄ¿ÂŒ
    }
    return false;
}

//ÅÐ¶ÏÊÇ·ñÎª³£¹æÎÄŒþ
bool is_file_xml(const char *path)
{
    struct stat statbuf;
    if(lstat(path, &statbuf) ==0)
        return S_ISREG(statbuf.st_mode) != 0;//ÅÐ¶ÏÎÄŒþÊÇ·ñÎª³£¹æÎÄŒþ
    return false;
}

//ÅÐ¶ÏÊÇ·ñÊÇÌØÊâÄ¿ÂŒ
bool is_special_dir_xml(const char *path)
{
    return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}

//Éú³ÉÍêÕûµÄÎÄŒþÂ·Ÿ¶
void get_file_path_xml(const char *path, const char *file_name,  char *file_path)
{
    strcpy(file_path, path);
    if(file_path[strlen(path) - 1] != '/')
        strcat(file_path, "/");
    strcat(file_path, file_name);
}

void delete_file_xml(const char *path,int *count,char* xmlname)
{
    DIR *dir;
    struct dirent *dir_info;
    char file_path[PATH_MAX];
	char filename[128];
struct stat *buf=(struct stat *)malloc(sizeof(struct stat));
	//get_file_path_xml(path, dir_info->d_name, file_path);
    if(is_file_xml(path))
    {
        //remove(path);
      (*count)++;
	if(stat(path,buf)<0)
	{
		printf("stat error\n");
		return;
	}
	printf("size of the %s is %d\n",path,buf->st_size);
	//strncpy(pathname_xml,path,strlen(path)+1);
	//strcat(pathname_xml,".xml");	
	
	get_sonstr(path,filename);	
	
	add_count (xmlname, buf,filename);

        return;
    }
    if(is_dir_xml(path))
    {
        if((dir = opendir(path)) == NULL)
            return;
        while((dir_info = readdir(dir)) != NULL)
        {
           get_file_path_xml(path, dir_info->d_name, file_path);
            if(is_special_dir_xml(dir_info->d_name))
                continue;
		//********************filename 
         delete_file_xml(file_path,count,xmlname);
		//strncpy(filename,dir_info->d_name,strlen(dir_info->d_name)+1);
	///	delete_file_xml(filename,count,xmlname);
            //rmdir(file_path);
		//(*count)++;
        }
	if(closedir(dir)<0)
		printf("close directory %s error \n ",path);
    }
//printf("count:%d\n",*count);
	free(buf);
}



int creat_xml(char *x_file)
{
       xmlDocPtr doc = NULL; 
	//char *Con="Content";
	     /* document pointer */
       xmlNodePtr root_node = NULL, node = NULL, node1 = NULL; /* node pointers */                                                      
      
       //Creates a new document, a node and set it as a root node
       doc = xmlNewDoc(BAD_CAST "1.0");
       root_node = xmlNewNode(NULL, BAD_CAST "ListBucketResult");
	xmlNewProp(root_node, BAD_CAST "xmlns", BAD_CAST "http://s3.amazonaws.com/doc/2006-03-01/"); 
       xmlDocSetRootElement(doc, root_node);    
      
       //creates a new node, which is "attached" as child node of root_node node. 
	xmlNewChild(root_node, NULL, BAD_CAST "Name",BAD_CAST "quotes");
	xmlNewChild(root_node, NULL, BAD_CAST "Prefix",BAD_CAST "N");
	xmlNewChild(root_node, NULL, BAD_CAST "Marker",BAD_CAST "Ned");
	xmlNewChild(root_node, NULL, BAD_CAST "Markerkey",BAD_CAST "40");
      xmlNewChild(root_node, NULL, BAD_CAST "IsTruncated",BAD_CAST "false");
 
       // xmlNewProp() creates attributes, which is "attached" to an node.

/*
       node=xmlNewNode(NULL, BAD_CAST "Content");
	xmlAddChild(root_node, node);
	xmlNewChild(node, NULL, BAD_CAST "Key",BAD_CAST "Nelson");
	xmlNewChild(node, NULL, BAD_CAST "LastModified",BAD_CAST "2006-01-01T12:00:00.000Z");
	xmlNewChild(node, NULL, BAD_CAST "ETag",BAD_CAST "&quot;828ef3fdfa96f00ad9f27c383fc9ac7f&quot;");
	xmlNewChild(node, NULL, BAD_CAST "Size",BAD_CAST "5");
	xmlNewChild(node, NULL, BAD_CAST "StorageClass",BAD_CAST "STANDARD");
       //xmlNewProp(node, BAD_CAST "attribute", BAD_CAST "yes");  
//Here goes another way to create nodes.
       node1 = xmlNewNode(NULL, BAD_CAST "Owner");
       //node1 = xmlNewText(BAD_CAST"other way to create content");
       xmlAddChild(node, node1);
	xmlNewChild(node1, NULL, BAD_CAST "ID",BAD_CAST "bcaf161ca5fb16fd081034f");
	xmlNewChild(node, NULL, BAD_CAST "DisplayName",BAD_CAST "webfile");
       //xmlAddChild(root_node, node);  
							*/                          
//Dumping document to stdio or file
       xmlSaveFormatFileEnc(x_file, doc, "UTF-8", 1);  
/*free the document */
       xmlFreeDoc(doc);
       xmlCleanupParser();
       xmlMemoryDump();      //debug memory for regression tests
 
       return 1;
}


//extern void
void add_count (char*DocName, struct stat *buf,char *filename) {

	//xmlNewTextChild (cur, NULL, "haha", keyword);
/*	
	xmlNodePtr son_node; 	
	son_node = xmlNewNode(NULL, BAD_CAST "ssss");
	xmlAddChild(cur, son_node);
	xmlNewTextChild (son_node, NULL, "niu", keyword);
	xmlNewTextChild (son_node, NULL, "bb", keyword);*/
	xmlNodePtr son_node,node,cur; 
	
	xmlDocPtr doc;   //
  // xmlNodePtr curNode;  // 
   
	//doc = xmlReadFile(DocName,"GB2312",XML_PARSE_RECOVER); 
	doc = xmlReadFile(DocName,"UTF—8",XML_PARSE_RECOVER);    
      // UTF—8
    if (NULL == doc)    
    {      
        fprintf(stderr,"Document not parsed successfully. \n");        
        return;    
    }    
       
    cur = xmlDocGetRootElement(doc);    
   
    if (NULL == cur)    
    {    
        fprintf(stderr,"empty document\n");    
        xmlFreeDoc(doc);    
        return;    
    }    
   


	//xmlNodePtr son_node,node; 
	node=xmlNewNode(NULL, BAD_CAST "Content");
	xmlAddChild(cur, node);
	//printf("success add Content\n");
	//xmlNewChild(node, NULL, BAD_CAST "FileName",BAD_CAST "Nelson");	
	xmlNewChild(node, NULL, BAD_CAST "Key",BAD_CAST filename);
	//printf("success add key\n");
	/* if (NULL == buf )
            printf("buf is NULL \n");
         else 
               printf("buf addr:%d\n", *(int *)&buf);
	*/
	xmlNewChild(node, NULL, BAD_CAST "LastModified",BAD_CAST ctime(&(buf->st_mtime)));
	printf("buf->st_mtime:%d    %s   \n ",buf->st_mtime,BAD_CAST ctime(&(buf->st_mtime)));
	//printf("success add lastmodified\n");
	xmlNewChild(node, NULL, BAD_CAST "ETag",BAD_CAST "&quot;828ef3fdfa96f00ad9f27c383fc9ac7f&quot;");
	xmlNewChild(node, NULL, BAD_CAST "Size",BAD_CAST my_itoa(buf->st_size));
	xmlNewChild(node, NULL, BAD_CAST "StorageClass",BAD_CAST "STANDARD");
       //xmlNewProp(node, BAD_CAST "attribute", BAD_CAST "yes");  
//Here goes another way to create nodes.
       son_node = xmlNewNode(NULL, BAD_CAST "Owner");
       //node1 = xmlNewText(BAD_CAST"other way to create content");
       xmlAddChild(node, son_node);
	xmlNewChild(son_node, NULL, BAD_CAST "ID",BAD_CAST "bcaf161ca5fb16fd081034f");
	xmlNewChild(son_node, NULL, BAD_CAST "DisplayName",BAD_CAST "webfile");
       //xmlAddChild(root_node, node);  
	int nRel = xmlSaveFile(DocName,doc);   
   if (nRel != -1)   
   {   
      // printf("sucess \n");   
printf("success write %d bytes\n");
    }   
    xmlFreeDoc(doc);    	
	//printf("success add 1 file Content\n");
	return;
}


char * my_itoa(int val)

{

    const int radix = 10;

    char* p;
    int a;        //every digit
    int len;
    char* b;    //start of the digit char
    char temp;
	char *buf=(char*)malloc(128*sizeof(char));
    p = buf;

    if (val < 0)
    {
        *p++ = '-';
        val = 0 - val;
    }

    b = p;

    do
    {
        a = val % radix;
        val /= radix;

        *p++ = a + '0';

    } while (val > 0);

    len = (int)(p - buf);

    *p-- = 0;

    //swap
    do
    {
        temp = *p;
        *p = *b;
        *b = temp;
        --p;
        ++b;

    } while (b < p);

    return buf;
}


int get_sonstr(char *path,char *filename)
{
	int i=0,point=0;	
	//char *filename=(char*)malloc(128*sizeof(char));
	while(path[i]!='\0')
	{	
		if(path[i]=='/')
		{
			point=i+1;
			//i++;
		}	
		i++;
	}
	i=0;
	while(path[i]!='\0')
	{
		filename[i]=path[i+point];
		i++;
	}
	filename[i]='\0';
	return 1;

}


//***********************************************
//judge object_or_bucket_in_put and in post
//return 1 means bucket and return 0 means object

int judge_object_or_bucket_in_put(char *filename)
{
	char dot='.';
	int i=0;
	while(filename[i]!=dot&&filename[i]!='\0') 
	{
		i++;

	}
	if(filename[i]=='\0')
	{
		printf("37 ****%s is a bucket!\n",__func__);
		return 1;
	}
	else
	{
		printf("%s is a object!\n");
		return 0;
	}
}

