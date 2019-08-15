#ifndef UPLOAD_FILE_H
#define UPLOAD_FILE_H

typedef struct post_file
{
    //文件名
  char *name;
  //content-type
  char *type;
  //数据域
  char *data;
  //数据域的长度
  int   datalen;
} PostFile;
char* memstr(char* full_data, int full_data_len, char* substr);
int PostFile_GetByResponseStr(PostFile *file,char *src,int len);
void PostFile_free(PostFile *pf);
#endif // UPLOAD_FILE_H
