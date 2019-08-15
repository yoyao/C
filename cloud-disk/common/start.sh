#!/bin/bash

echo "login start..."
spawn-fcgi -a 127.0.0.1 -p 8002 -f /home/fgo/document/course/cloud-disk/common/login.out
echo "filelist start..."
spawn-fcgi -a 127.0.0.1 -p 8003 -f /home/fgo/document/course/cloud-disk/common/getfilelist.out
echo "filecheck start..."
spawn-fcgi -a 127.0.0.1 -p 8008 -f /home/fgo/document/course/cloud-disk/common/filecheck.out
#echo "fileupload start..."
#spawn-fcgi -a 127.0.0.1 -p 8005 -f /home/fgo/document/course/cloud-disk/common/fileupload.out
