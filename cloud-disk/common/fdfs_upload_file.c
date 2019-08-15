
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fastdfs/fdfs_client.h>
#include <fastcommon/logger.h>
#include <fastcommon/shared_func.h>

#define CONF_FILE_NAME "/etc/fdfs/client.conf"

int UploadFile(const char *local_filename)
{
    if(local_filename ==NULL)
    {
        puts("UploadFile:local_filename Not access NULL");
        return 0;
    }
    char group_name[FDFS_GROUP_NAME_MAX_LEN + 1];
    ConnectionInfo *pTrackerServer;
    int result;
    int store_path_index;
    ConnectionInfo storageServer;
    char file_id[128];
    log_init();
    g_log_context.log_level = LOG_ERR;
    ignore_signal_pipe();

    if ((result=fdfs_client_init(CONF_FILE_NAME)) != 0)
    {
        return result;
    }

    pTrackerServer = tracker_get_connection();
    if (pTrackerServer == NULL)
    {
        fdfs_client_destroy();
        return errno != 0 ? errno : ECONNREFUSED;
    }
    *group_name = '\0';
    if ((result=tracker_query_storage_store(pTrackerServer, \
                                            &storageServer, group_name, &store_path_index)) != 0)
    {
        fdfs_client_destroy();
        fprintf(stderr, "tracker_query_storage fail, " \
                        "error no: %d, error info: %s\n", \
                result, STRERROR(result));
        return result;
    }

    result = storage_upload_by_filename1(pTrackerServer, \
                                         &storageServer, store_path_index,
                                         local_filename, NULL, \
                                         NULL, 0, group_name, file_id);
    if (result == 0)
    {
        printf("%s\n", file_id);
    }
    else
    {
        fprintf(stderr, "upload file fail, " \
                        "error no: %d, error info: %s\n", \
                result, STRERROR(result));
    }

    tracker_disconnect_server_ex(pTrackerServer, true);
    fdfs_client_destroy();

    return result;
}

int main(int argc, char *argv[])
{
    UploadFile(argv[1]);
    return 0;
}
