##################################################
SRCROOT	:= $(abspath ..)

ifdef debug
	MACROS += _CC_USE_CC_DEBUG_MALLOC_
endif

#指定第三方库安装路径
THIRD_PARTY_PATH = $(abspath $(SRCROOT)/..)/third-party
INCLUDE_PATH += $(THIRD_PARTY_PATH)
LIBRARY_PATH += $(THIRD_PARTY_PATH)/lib/$(ARCH)/

include $(SRCROOT)/build/Makefile.mak

ifdef target
	include $(SRCROOT)/build/$(target).mak
endif