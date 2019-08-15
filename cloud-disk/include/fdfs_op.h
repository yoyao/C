#ifndef FDFS_OP_H
#define FDFS_OP_H

#include <fastdfs/client_func.h>
#include <fastcommon/md5.h>
#include <fastdfs/fdfs_client.h>
#include <fastcommon/logger.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int UploadFile2Fdfs(const char *local_filename,char *out_filename);


#endif // FDFS_OP_H
