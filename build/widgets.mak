#make .dll target=widgets url_request=1 db=1 debug=1
WIDGET_FILES = $(SRCROOT)/src/widgets
TARGET_NAME = cc.$(target)

ifdef all
	db = 1
	url_request = 1
endif

MACROS	+= _CC_USE_OPENSSL_=1
MACROS	+= _CC_API_WIDGETS_USE_DYNAMIC_=1

LIBS	+= cc

ifdef debug
	LIBS += cc.debug.malloc
endif

ifeq ($(PLATFORM), windows)
	LIBS += ssl-3-x64 crypto-3-x64
else ifeq ($(PLATFORM), osx)
	LIBS += ssl.3 crypto.3
else
	LIBS += ssl crypto
endif

ifdef db
	LIBS += sqlite3 
	ifeq ($(PLATFORM), windows)
		LIBS += mysql
	else
		LIBS += mysqlclient
	endif
endif

ifeq ($(PLATFORM), osx)
	INCLUDE_PATH	+= /opt/homebrew/include
	LIBRARY_PATH	+= /opt/homebrew/lib
endif

ifdef db
ifeq ($(PLATFORM), osx)
	MACROS			+= _CC_USE_UNIXODBC_=1

	LOCAL_SRC_FILES += $(SRCROOT)/src/widgets/db/sqlsvr.o

	LIBS			+= odbc

	INCLUDE_PATH	+= /opt/homebrew/opt/mysql-client/include
	LIBRARY_PATH	+= /opt/homebrew/opt/mysql-client/lib
endif

INCLUDE_PATH	+= /usr/local/include /usr/include
LIBRARY_PATH	+= /usr/local/lib /usr/local/lib/mysql

LOCAL_SRC_FILES += \
					$(WIDGET_FILES)/db/sqlite.o \
					$(WIDGET_FILES)/db/mysql.o
endif # --end db--

LOCAL_SRC_FILES += \
					$(WIDGET_FILES)/event/event.o \
					$(WIDGET_FILES)/event/loop.o \
					$(WIDGET_FILES)/event/select.o \
					$(WIDGET_FILES)/event/timeout.o \
					$(WIDGET_FILES)/event/buffer.o \
					$(WIDGET_FILES)/event/tcp.o \
					$(WIDGET_FILES)/event/OpenSSL.o

ifeq ($(PLATFORM), linux)
LOCAL_SRC_FILES += $(WIDGET_FILES)/event/linux/sys_epoll.o
#LOCAL_SRC_FILES += $(WIDGET_FILES)/event/linux/sys_io_uring.o
else ifeq ($(PLATFORM), windows)
LOCAL_SRC_FILES += \
					$(WIDGET_FILES)/event/windows/sys_WSA.o \
					$(WIDGET_FILES)/event/windows/sys_iocp.o \
					$(WIDGET_FILES)/event/windows/sys_io_context.o
else ifeq ($(PLATFORM), osx)
LOCAL_SRC_FILES += $(WIDGET_FILES)/event/unix/sys_kqueue.o
endif

LOCAL_SRC_FILES += \
					$(WIDGET_FILES)/json/json.o \
					$(WIDGET_FILES)/json/json.array.o \
					$(WIDGET_FILES)/json/json.object.o \
					$(WIDGET_FILES)/json/json.parser.o

LOCAL_SRC_FILES += \
					$(WIDGET_FILES)/ini/ini.o \
					$(WIDGET_FILES)/ini/ini.parser.o

LOCAL_SRC_FILES += \
					$(WIDGET_FILES)/xml/xml.o \
					$(WIDGET_FILES)/xml/xml.parser.o


ifdef url_request
LOCAL_SRC_FILES += \
					$(WIDGET_FILES)/generic/gzip.o \
					$(WIDGET_FILES)/http/header.o \
					$(WIDGET_FILES)/http/request.parser.o \
					$(WIDGET_FILES)/http/response.parser.o \
					$(WIDGET_FILES)/url_request/http/url_request.o \
					$(WIDGET_FILES)/url_request/http/url_response.o
endif # --end url_request --

#LOCAL_SRC_FILES += \
					$(WIDGET_FILES)/ip_locator/ip_locator.o

LOCAL_SRC_FILES += \
					$(WIDGET_FILES)/smtp/libsmtp.o \
					$(WIDGET_FILES)/smtp/connected.o \
					$(WIDGET_FILES)/smtp/login.o \
					$(WIDGET_FILES)/smtp/from_rcpt.o

#LOCAL_SRC_FILES += \
					$(WIDGET_FILES)/ftp/libftp.o \
					$(WIDGET_FILES)/ftp/connected.o \
					$(WIDGET_FILES)/ftp/login.o \
					$(WIDGET_FILES)/ftp/cdup.o \
					$(WIDGET_FILES)/ftp/cwd.o \
					$(WIDGET_FILES)/ftp/delete_file.o \
					$(WIDGET_FILES)/ftp/delete_folder.o \
					$(WIDGET_FILES)/ftp/list.o \
					$(WIDGET_FILES)/ftp/mkdir.o \
					$(WIDGET_FILES)/ftp/opts.o \
					$(WIDGET_FILES)/ftp/rename_file.o

#LOCAL_SRC_FILES += \
					$(WIDGET_FILES)/dns/dns.o \
					$(WIDGET_FILES)/dns/dnss.o

LOCAL_SRC_FILES += \
					$(WIDGET_FILES)/generic/generic.o \
					$(WIDGET_FILES)/generic/WS.o \
					$(WIDGET_FILES)/widgets.o \
					$(WIDGET_FILES)/main.o