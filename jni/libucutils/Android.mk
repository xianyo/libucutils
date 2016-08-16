
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:= libucutils

include ${LOCAL_PATH}/config.mk

# toolbox core sources
LOCAL_SRC_FILES :=  toolbox/memory.c
LOCAL_SRC_FILES += toolbox/misc.c
LOCAL_SRC_FILES += toolbox/str.c
LOCAL_SRC_FILES += toolbox/buf.c
LOCAL_SRC_FILES += toolbox/log.c
LOCAL_SRC_FILES += toolbox/lexer.c

ifndef NO_TEST
    LOCAL_SRC_FILES += toolbox/test.c
endif
ifndef NO_HMAP
    LOCAL_SRC_FILES += toolbox/hmap.c
endif
ifndef NO_CONFIG
    LOCAL_SRC_FILES += toolbox/config.c
    LOCAL_SRC_FILES += toolbox/config_fs.c
endif
ifndef NO_ENV
    LOCAL_SRC_FILES += toolbox/env.c
endif
ifndef NO_NET
    LOCAL_SRC_FILES += toolbox/net.c 
    LOCAL_SRC_FILES += toolbox/uri.c
endif
ifndef NO_FS
    LOCAL_SRC_FILES += toolbox/fs.c
endif
ifndef NO_B64
    LOCAL_SRC_FILES += toolbox/b64.c
endif
ifndef NO_PWD
    ifdef NO_HMAP   # pwd needs hmap
        $(warning adding hmap module as a dependency to pwd)
        LOCAL_SRC_FILES += toolbox/hmap.c
    endif   # NO_HMAP (PWD dep)
    LOCAL_SRC_FILES += toolbox/pwd.c
endif   # !NO_PWD 
ifndef NO_LIST
    LOCAL_SRC_FILES += toolbox/list.c
endif
ifndef NO_ARRAY
    LOCAL_SRC_FILES += toolbox/array.c
endif
ifndef NO_RB
    LOCAL_SRC_FILES += toolbox/rb.c
endif
ifndef NO_PQUEUE
    LOCAL_SRC_FILES += toolbox/pqueue.c
endif
ifndef NO_BST
    LOCAL_SRC_FILES += toolbox/bst.c
endif
ifndef NO_JSON
    ifdef NO_HMAP   # json needs hmap
        $(warning adding hmap module as a dependency to json)
        LOCAL_SRC_FILES += toolbox/hmap.c
    endif   # NO_HMAP (JSON dep)
    LOCAL_SRC_FILES += toolbox/json.c
endif
ifdef SHLIB_NO_UNDEFINED_SYMS
    LOCAL_SRC_FILES += toolbox/facility.c
endif

ifdef HAVE_LIBSERIALPORT
    LOCAL_SRC_FILES += lib/libserialport/linux.c
    LOCAL_SRC_FILES += lib/libserialport/linux_termios.c
    LOCAL_SRC_FILES += lib/libserialport/serialport.c
endif

ifdef HAVE_LIBHAL
    LOCAL_SRC_FILES += lib/libhal/hardware.c
endif

ifdef HAVE_LIBMYUTILS
    LOCAL_SRC_FILES += lib/libmyutils/myutils.c
endif

# missing sources
ifndef HAVE_VSYSLOG
    LOCAL_SRC_FILES += missing/vsyslog.c
endif
ifndef HAVE_SYSLOG
    LOCAL_SRC_FILES += missing/syslog.c
endif
ifndef HAVE_STRTOK_R
    LOCAL_SRC_FILES += missing/strtok_r.c
endif
ifndef HAVE_UNLINK
    LOCAL_SRC_FILES += missing/unlink.c
endif
ifndef HAVE_GETPID
    LOCAL_SRC_FILES += missing/getpid.c
endif
ifndef HAVE_FNMATCH
    LOCAL_SRC_FILES += missing/fnmatch.c
endif
ifndef HAVE_TIMEGM
    LOCAL_SRC_FILES += missing/timegm.c
endif
ifndef HAVE_STRSEP
    LOCAL_SRC_FILES += missing/strsep.c
endif
ifndef HAVE_STRLCPY
    LOCAL_SRC_FILES += missing/strlcpy.c
endif
ifndef HAVE_STRLCAT
    LOCAL_SRC_FILES += missing/strlcat.c
endif
ifndef HAVE_GETTIMEOFDAY
    LOCAL_SRC_FILES += missing/gettimeofday.c
endif
ifndef HAVE_DAEMON
    LOCAL_SRC_FILES += missing/daemon.c
endif
ifndef HAVE_MKSTEMPS
    LOCAL_SRC_FILES += missing/mkstemps.c
endif
ifndef HAVE_SETENV
    LOCAL_SRC_FILES += missing/setenv.c
endif

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/u \
                    $(LOCAL_PATH)/include

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_EXPORT_LDLIBS := -llog -landroid
LOCAL_LDLIBS := -llog 

#hard-fp setting
ifneq ($(filter %armeabi-v7a,$(TARGET_ARCH_ABI)),)
#For now, only armeabi-v7a is supported for hard-fp
#adding compiler/liker flags specifying hard float ABI for user code and math library
LOCAL_CFLAGS += -mhard-float -D_NDK_MATH_NO_SOFTFP=1
LOCAL_EXPORT_CFLAGS += -mhard-float -D_NDK_MATH_NO_SOFTFP=1
LOCAL_EXPORT_LDLIBS += -lm_hard
ifeq (,$(filter -fuse-ld=mcld,$(APP_LDFLAGS) $(LOCAL_LDFLAGS)))
#Supressing warn-mismatch warnings
LOCAL_EXPORT_LDFLAGS += -Wl,--no-warn-mismatch
endif
endif

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)

