ifeq ($(PLATFORM), osx)
	INCLUDE_PATH	+= /opt/homebrew/include
	LIBRARY_PATH	+= /opt/homebrew/lib
endif

INCLUDE_PATH	+= $(LIBCC_SRC_ROOT)/include
LIBRARY_PATH	+= $(LIBCC_SRC_ROOT)/lib

ifdef libopenssl
	ifeq ($(PLATFORM), windows)
		LIBS += ssl-3-x64 crypto-3-x64
	else ifeq ($(PLATFORM), osx)
		LIBS += ssl.3 crypto.3
	else
		LIBS += ssl crypto
	endif
	MACROS	+= _CC_USE_OPENSSL_=1
endif # --end libopenssl--

INCLUDE_PATH	+= /usr/local/include /usr/include

ifdef libmysql
	ifeq ($(PLATFORM), osx)
		MACROS			+= _CC_USE_UNIXODBC_=1
		LIBS			+= odbc mysqlclient
		INCLUDE_PATH	+= /opt/homebrew/opt/mysql-client/include
		LIBRARY_PATH	+= /opt/homebrew/opt/mysql-client/lib
	else ifeq ($(PLATFORM), windows)
		LIBS += mysql
	else
		LIBS += mysqlclient
		LIBRARY_PATH	+= /usr/local/lib /usr/local/lib/mysql
	endif
endif # --end libmysql--

ifdef libsqlite3
	LIBS += sqlite3 
endif # --end libsqlite3--

ifdef shared
	MACROS += _CC_API_USE_DYNAMIC_
endif

ifdef debug
	MACROS += _CC_USE_DEBUG_MALLOC_
endif

#MACROS += _CC_SYSLOG_RFC5424_

LOCAL_SRC_FILES = \
	$(LIBCC_SRC_ROOT)/src/crypto/aes.o \
	$(LIBCC_SRC_ROOT)/src/crypto/base16.o \
	$(LIBCC_SRC_ROOT)/src/crypto/base58.o \
	$(LIBCC_SRC_ROOT)/src/crypto/base64.o \
	$(LIBCC_SRC_ROOT)/src/crypto/md2.o \
	$(LIBCC_SRC_ROOT)/src/crypto/md4.o \
	$(LIBCC_SRC_ROOT)/src/crypto/md5.o \
	$(LIBCC_SRC_ROOT)/src/crypto/sha1.o \
	$(LIBCC_SRC_ROOT)/src/crypto/sha256.o \
	$(LIBCC_SRC_ROOT)/src/crypto/sha512.o \
	$(LIBCC_SRC_ROOT)/src/crypto/hmac.o \
	$(LIBCC_SRC_ROOT)/src/crypto/des.o \
	$(LIBCC_SRC_ROOT)/src/crypto/xxtea.o \
	$(LIBCC_SRC_ROOT)/src/crc.o \
	$(LIBCC_SRC_ROOT)/src/UTF.o \
	$(LIBCC_SRC_ROOT)/src/GBK.o \
	$(LIBCC_SRC_ROOT)/src/list.o \
	$(LIBCC_SRC_ROOT)/src/queue.o \
	$(LIBCC_SRC_ROOT)/src/hmap.o \
	$(LIBCC_SRC_ROOT)/src/rbtree.o \
	$(LIBCC_SRC_ROOT)/src/array.o \
	$(LIBCC_SRC_ROOT)/src/string.o \
	$(LIBCC_SRC_ROOT)/src/sds.o \
	$(LIBCC_SRC_ROOT)/src/buf.o \
	$(LIBCC_SRC_ROOT)/src/uuid.o \
	$(LIBCC_SRC_ROOT)/src/url.o \
	$(LIBCC_SRC_ROOT)/src/rand.o \
	$(LIBCC_SRC_ROOT)/src/ring.o \
	$(LIBCC_SRC_ROOT)/src/logger.o \
	$(LIBCC_SRC_ROOT)/src/syslog.o \
	$(LIBCC_SRC_ROOT)/src/cpu.o \
	$(LIBCC_SRC_ROOT)/src/file.o \
	$(LIBCC_SRC_ROOT)/src/malloc/alloc.o \
	$(LIBCC_SRC_ROOT)/src/malloc/debug.malloc.o \
	$(LIBCC_SRC_ROOT)/src/malloc/debug.tracked.o \
	$(LIBCC_SRC_ROOT)/src/thread/thread.o \
	$(LIBCC_SRC_ROOT)/src/atomic/atomic.o \
	$(LIBCC_SRC_ROOT)/src/atomic/rwlock.o \
	$(LIBCC_SRC_ROOT)/src/platform/platform.o \
	$(LIBCC_SRC_ROOT)/src/event/event.o \
	$(LIBCC_SRC_ROOT)/src/event/loop.o \
	$(LIBCC_SRC_ROOT)/src/event/select.o \
	$(LIBCC_SRC_ROOT)/src/event/timeout.o \
	$(LIBCC_SRC_ROOT)/src/event/buffer.o \
	$(LIBCC_SRC_ROOT)/src/event/tcp.o \
	$(LIBCC_SRC_ROOT)/src/event/OpenSSL.o \
	$(LIBCC_SRC_ROOT)/src/json/json.o \
	$(LIBCC_SRC_ROOT)/src/json/json.array.o \
	$(LIBCC_SRC_ROOT)/src/json/json.object.o \
	$(LIBCC_SRC_ROOT)/src/json/json.parser.o \
	$(LIBCC_SRC_ROOT)/src/ini/ini.o \
	$(LIBCC_SRC_ROOT)/src/ini/ini.parser.o\
	$(LIBCC_SRC_ROOT)/src/xml/xml.o \
	$(LIBCC_SRC_ROOT)/src/xml/xml.parser.o \
	$(LIBCC_SRC_ROOT)/src/generic/strptime.o \
	$(LIBCC_SRC_ROOT)/src/generic/tick.o \
	$(LIBCC_SRC_ROOT)/src/generic/time.o \
	$(LIBCC_SRC_ROOT)/src/generic/inet.o \
	$(LIBCC_SRC_ROOT)/src/generic/socket.o \
	$(LIBCC_SRC_ROOT)/src/generic/power.o  \
	$(LIBCC_SRC_ROOT)/src/generic/generic.o \
	$(LIBCC_SRC_ROOT)/src/generic/WS.o \
	$(LIBCC_SRC_ROOT)/src/main.o

ifneq ($(filter $(PLATFORM), freebsd unix),)
	LOCAL_SRC_FILES += \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_time.o \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_unix.o \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_dirent.o \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_locale.o \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_socket.o \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_loadso.o \
		$(LIBCC_SRC_ROOT)/src/thread/pthread/sys_thread.o \
		$(LIBCC_SRC_ROOT)/src/thread/pthread/sys_cond.o \
		$(LIBCC_SRC_ROOT)/src/thread/pthread/sys_mutex.o \
		$(LIBCC_SRC_ROOT)/src/thread/pthread/sys_sem.o \
		$(LIBCC_SRC_ROOT)/src/event/unix/sys_kqueue.o
endif

ifneq ($(filter $(PLATFORM), osx),)
		LOCAL_SRC_FILES += \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_time.o \
		$(LIBCC_SRC_ROOT)/src/platform/OSX/sys_file.o \
		$(LIBCC_SRC_ROOT)/src/platform/OSX/sys_dirent.o \
		$(LIBCC_SRC_ROOT)/src/platform/OSX/sys_locale.o \
		$(LIBCC_SRC_ROOT)/src/platform/OSX/sys_osx.o \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_unix.o \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_socket.o \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_loadso.o \
		$(LIBCC_SRC_ROOT)/src/platform/osx/sys_power.o \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_kqueue.o \
		$(LIBCC_SRC_ROOT)/src/thread/pthread/sys_thread.o \
		$(LIBCC_SRC_ROOT)/src/thread/pthread/sys_cond.o \
		$(LIBCC_SRC_ROOT)/src/thread/apple/sys_mutex.o \
		$(LIBCC_SRC_ROOT)/src/thread/apple/sys_sem.o
endif

ifneq ($(filter $(PLATFORM), ios),)
	LOCAL_SRC_FILES += \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_time.o \
		$(LIBCC_SRC_ROOT)/src/platform/IOS/sys_file.o \
		$(LIBCC_SRC_ROOT)/src/platform/IOS/sys_dirent.o \
		$(LIBCC_SRC_ROOT)/src/platform/IOS/sys_ios.o \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_unix.o \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_socket.o \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_kqueue.o \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_poll.o \
		$(LIBCC_SRC_ROOT)/src/thread/pthread/sys_thread.o \
		$(LIBCC_SRC_ROOT)/src/thread/pthread/sys_mutex.o \
		$(LIBCC_SRC_ROOT)/src/thread/pthread/sys_cond.o \
		$(LIBCC_SRC_ROOT)/src/thread/apple/sys_sem.o
endif

ifeq ($(PLATFORM), linux)
	LOCAL_SRC_FILES += \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_unix.o \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_dirent.o \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_loadso.o \
		$(LIBCC_SRC_ROOT)/src/platform/unix/sys_locale.o \
		$(LIBCC_SRC_ROOT)/src/platform/linux/sys_time.o \
		$(LIBCC_SRC_ROOT)/src/platform/linux/sys_power.o \
		$(LIBCC_SRC_ROOT)/src/platform/linux/sys_socket.o \
		$(LIBCC_SRC_ROOT)/src/platform/linux/sys_epoll.o \
		$(LIBCC_SRC_ROOT)/src/thread/pthread/sys_thread.o \
		$(LIBCC_SRC_ROOT)/src/thread/pthread/sys_cond.o \
		$(LIBCC_SRC_ROOT)/src/thread/pthread/sys_mutex.o \
		$(LIBCC_SRC_ROOT)/src/thread/pthread/sys_sem.o
		#$(LIBCC_SRC_ROOT)/src/platform/linux/sys_io_uring.o
endif

ifeq ($(PLATFORM), windows)
	LOCAL_SRC_FILES += \
		$(LIBCC_SRC_ROOT)/src/platform/windows/sys_time.o \
		$(LIBCC_SRC_ROOT)/src/platform/windows/sys_windows.o \
		$(LIBCC_SRC_ROOT)/src/platform/windows/sys_mmap.o \
		$(LIBCC_SRC_ROOT)/src/platform/windows/sys_dirent.o \
		$(LIBCC_SRC_ROOT)/src/platform/windows/sys_file.o \
		$(LIBCC_SRC_ROOT)/src/platform/windows/sys_pipe.o \
		$(LIBCC_SRC_ROOT)/src/platform/windows/sys_locale.o \
		$(LIBCC_SRC_ROOT)/src/platform/windows/sys_power.o \
		$(LIBCC_SRC_ROOT)/src/platform/windows/sys_socket.o \
		$(LIBCC_SRC_ROOT)/src/platform/windows/sys_loadso.o \
		$(LIBCC_SRC_ROOT)/src/thread/windows/sys_thread.o \
		$(LIBCC_SRC_ROOT)/src/thread/windows/sys_cond.o \
		$(LIBCC_SRC_ROOT)/src/thread/windows/sys_mutex.o \
		$(LIBCC_SRC_ROOT)/src/thread/windows/sys_sem.o \
		$(LIBCC_SRC_ROOT)/src/platform/windows/sys_WSA.o \
		$(LIBCC_SRC_ROOT)/src/platform/windows/sys_iocp.o \
		$(LIBCC_SRC_ROOT)/src/platform/windows/sys_io_context.o
endif

ifdef libsqlite3
	LOCAL_SRC_FILES += \
		$(LIBCC_SRC_ROOT)/src/db/sqlite.o
endif # --end libsqlite3--

ifdef libmysql
	LOCAL_SRC_FILES += \
		$(LIBCC_SRC_ROOT)/src/db/mysql.o
		#$(LIBCC_SRC_ROOT)/src/db/sqlsvr.o 
endif # --end libmysql--

ifdef liburl_request
	LOCAL_SRC_FILES += \
		$(LIBCC_SRC_ROOT)/src/generic/gzip.o \
		$(LIBCC_SRC_ROOT)/src/http/header.o \
		$(LIBCC_SRC_ROOT)/src/http/request.parser.o \
		$(LIBCC_SRC_ROOT)/src/http/response.parser.o \
		$(LIBCC_SRC_ROOT)/src/url_request/http/url_request.o \
		$(LIBCC_SRC_ROOT)/src/url_request/http/url_response.o
endif # --end liburl_request --

ifdef libsmtp
	LOCAL_SRC_FILES += \
		$(LIBCC_SRC_ROOT)/src/smtp/libsmtp.o \
		$(LIBCC_SRC_ROOT)/src/smtp/connected.o \
		$(LIBCC_SRC_ROOT)/src/smtp/login.o \
		$(LIBCC_SRC_ROOT)/src/smtp/from_rcpt.o
endif # --end libsmtp --

ifdef libftp
#LOCAL_SRC_FILES += \
		$(LIBCC_SRC_ROOT)/src/ftp/libftp.o \
		$(LIBCC_SRC_ROOT)/src/ftp/connected.o \
		$(LIBCC_SRC_ROOT)/src/ftp/login.o \
		$(LIBCC_SRC_ROOT)/src/ftp/cdup.o \
		$(LIBCC_SRC_ROOT)/src/ftp/cwd.o \
		$(LIBCC_SRC_ROOT)/src/ftp/delete_file.o \
		$(LIBCC_SRC_ROOT)/src/ftp/delete_folder.o \
		$(LIBCC_SRC_ROOT)/src/ftp/list.o \
		$(LIBCC_SRC_ROOT)/src/ftp/mkdir.o \
		$(LIBCC_SRC_ROOT)/src/ftp/opts.o \
		$(LIBCC_SRC_ROOT)/src/ftp/rename_file.o
endif # --end libftp --

ifdef libdns
	#LOCAL_SRC_FILES += \
		$(LIBCC_SRC_ROOT)/src/dns/dns.o \
		$(LIBCC_SRC_ROOT)/src/dns/dnss.o
endif # --end libdns --