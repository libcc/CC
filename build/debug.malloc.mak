DEBUG_MALLOC_FILES = $(SRCROOT)/src
TARGET_NAME = cc.$(target)

MACROS	+= _CC_API_USE_DYNAMIC_=1 _CC_USE_DEBUG_MALLOC_=1

LOCAL_SRC_FILES += \
					$(DEBUG_MALLOC_FILES)/atomic/rwlock.o \
					$(DEBUG_MALLOC_FILES)/atomic/atomic.o \
					$(DEBUG_MALLOC_FILES)/core/cpu_info.o \
					$(DEBUG_MALLOC_FILES)/debug.malloc/debug.malloc.o \
					$(DEBUG_MALLOC_FILES)/debug.malloc/debug.tracked.o