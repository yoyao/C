TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    common/redis_op.c \
    common/make_log.c \
    common/mysql_api.c \
    fdfs_op.c \
    common/fdfs_op.c \
    common/upload_file.c \
    common/userinfo_op.c \
    common/fileinfo_op.c \
    common/mysql_op.c \
    common/login.c \
    common/cJSON.c \
    common/getfilelist.c \
    common/filecheck.c \
    common/fileupload.c \
    common/thread.c \
    common/uploadfile.c \
    user.cpp \
    model/user.cpp \
    tools/base64.cpp \
    common/fdfs_upload_file.c \
    common/SocketClientEx.cc \
    fcgi_echo.c \
    tcpauth/client-proxy.c \
    tcpauth/common.c \
    tcpauth/wrap.c \
    model/UserInfo.pb.cpp \
    common/socketservice.cpp \
    tools/socketclientex.cpp \
    main.cpp \
    tools/socketservice.cpp

HEADERS += \
    include/redis_op.h \
    include/make_log.h \
    fdfs_op.h \
    include/fdfs_op.h \
    include/upload_file.h \
    include/userinfo_op.h \
    include/fileinfo_op.h \
    include/mysql_op.h \
    include/mysql_op.h \
    include/cJSON.h \
    user.h \
    model/user.h \
    tools/base64.h \
    model/UserInfo.pb.h \
    tcpauth/common.h \
    include/socketservice.h \
    include/socketclientex.h \
    tools/socketclientex.h

DISTFILES += \
    tcpauth/README.md
