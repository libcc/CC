##################################################
#make .dll liburl_request=1 libmysql=1 libsmtp=1 libopenssl=1
SRCROOT	:= $(abspath .)
#生成的文件名
TARGET_NAME ?= cc

LIBCC_SRC_ROOT := $(SRCROOT)

liburl_request=1 
libmysql=1
libsqlite3=1
libsmtp=1 
libopenssl=1

include $(SRCROOT)/build/Makefile.mak
include $(SRCROOT)/build/libcc-files.mak
