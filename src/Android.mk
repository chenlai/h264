LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(notdir $(wildcard $(LOCAL_PATH)/*.c))

LOCAL_C_INCLUDES:= \
	$(LOCAL_PATH)/../include
	
# Enable this to enable frame profiling using counters and timers
#LOCAL_CFLAGS += -DENABLE_PROFILING

LOCAL_MODULE:= libh264dec

ifeq ($(TARGET_ARCH),arm)
    LOCAL_CFLAGS += -Wno-psabi
endif

include $(BUILD_STATIC_LIBRARY)

################################################################################


