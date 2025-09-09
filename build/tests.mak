# 定义源文件目录
SRC_TESTS_DIR := $(SRCROOT)/tests

LOCAL_SRC_FILES = $(SRC_TESTS_DIR)/test_$(build).o

TARGET_NAME = $(build)

LIBS	+= cc cc.widgets

ifdef debug
	LIBS += cc.debug.malloc
endif

ifdef dll
	LIBS += $(dll)
endif