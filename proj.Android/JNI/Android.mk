LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS += -D_CC_API_USE_DYNAMIC_

ifeq ($(NDK_DEBUG),1)
    LOCAL_CFLAGS +=  -DDEBUG
endif

ifeq ($(NDK_UNICODE), 1)
    LOCAL_CFLAGS +=  -DUNICODE
endif

LOCAL_STATIC_LIBRARIES += cpufeatures
APP_ALLOW_MISSING_DEPS := true

LOCAL_MODULE := libcc$(MODULE_NAME_EXT)
LOCAL_C_INCLUDES := ../../include ./
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_SRC_FILES := \
    ../../src/crypto/aes.c \
    ../../src/crypto/base16.c \
    ../../src/crypto/base58.c \
    ../../src/crypto/base64.c \
    ../../src/crypto/md2.c \
    ../../src/crypto/md4.c \
    ../../src/crypto/md5.c \
    ../../src/crypto/sha1.c \
    ../../src/crypto/sha256.c \
    ../../src/crypto/sha512.c \
    ../../src/crypto/hmac.c \
    ../../src/crypto/des.c \
    ../../src/crypto/xxtea.c \
    ../../src/crc.c \
    ../../src/UTF.c \
    ../../src/GBK.c \
    ../../src/list.c \
    ../../src/queue.c \
    ../../src/hmap.c \
    ../../src/rbtree.c \
    ../../src/array.c \
    ../../src/string.c \
    ../../src/sds.c \
    ../../src/buf.c \
    ../../src/uuid.c \
    ../../src/url.c \
    ../../src/rand.c \
    ../../src/ring.c \
    ../../src/logger.c \
    ../../src/syslog.c \
    ../../src/cpu.c \
    ../../src/file.c \
    ../../src/malloc/alloc.c \
    ../../src/malloc/debug.malloc.c \
    ../../src/malloc/debug.tracked.c \
    ../../src/thread/thread.c \
    ../../src/atomic/atomic.c \
    ../../src/atomic/rwlock.c \
    ../../src/event/event.c \
    ../../src/event/loop.c \
    ../../src/event/select.c \
    ../../src/event/timeout.c \
    ../../src/event/buffer.c \
    ../../src/event/tcp.c \
    ../../src/event/OpenSSL.c \
    ../../src/json/json.c \
    ../../src/json/json.array.c \
    ../../src/json/json.object.c \
    ../../src/json/json.parser.c \
    ../../src/ini/ini.c \
    ../../src/ini/ini.parser.c\
    ../../src/xml/xml.c \
    ../../src/xml/xml.parser.c \
    ../../src/generic/strptime.c \
    ../../src/generic/tick.c \
    ../../src/generic/time.c \
    ../../src/generic/inet.c \
    ../../src/generic/socket.c \
    ../../src/generic/power.c  \
    ../../src/generic/generic.c \
    ../../src/generic/WS.c \
    ../../src/generic/gzip.c \
    ../../src/http/header.c \
    ../../src/http/request.parser.c \
    ../../src/http/response.parser.c \
    ../../src/url_request/http/url_request.c \
    ../../src/url_request/http/url_response.c \
    ../../src/smtp/libsmtp.c \
    ../../src/smtp/connected.c \
    ../../src/smtp/login.c \
    ../../src/smtp/from_rcpt.c \
    ../../src/thread/pthread/sys_thread.c \
    ../../src/thread/pthread/sys_cond.c \
    ../../src/thread/pthread/sys_mutex.c \
    ../../src/thread/pthread/sys_sem.c \
    ../../src/platform/platform.c \
    ../../src/platform/linux/sys_epoll.c \
    ../../src/platform/linux/sys_socket.c \
    ../../src/platform/linux/sys_time.c \
    ../../src/platform/unix/sys_loadso.c \
    ../../src/platform/android/sys_clipboard.c \
    ../../src/platform/android/sys_android.c \
    ../../src/platform/android/sys_dirent.c \
    ../../src/platform/android/sys_file.c \
    ../../src/platform/android/sys_locale.c \
    ../../src/platform/android/sys_power.c \
    ../../src/main.c

LOCAL_EXPORT_LDLIBS := $(LOCAL_LDLIBS)
include $(BUILD_STATIC_LIBRARY)

# LOCAL_LDLIBS := -ldl -llog -landroid -lz
# include $(BUILD_SHARED_LIBRARY)
