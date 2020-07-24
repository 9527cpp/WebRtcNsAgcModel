#COMPILIE_PREFIX = arm-openwrt-linux-muslgnueabi-
CC = $(COMPILIE_PREFIX)g++
LD = $(COMPILIE_PREFIX)ld
AR = $(COMPILIE_PREFIX)ar

BASE_PATH=$(PWD)


# SRCS += ${wildcard *.c}
SRCS:=
SRCS_C:= \
	./common_audio/signal_processing/complex_bit_reverse.c \
	./common_audio/signal_processing/complex_fft.c \
	./common_audio/signal_processing/copy_set_operations.c \
	./common_audio/signal_processing/cross_correlation.c \
	./common_audio/signal_processing/division_operations.c \
	./common_audio/signal_processing/downsample_fast.c \
	./common_audio/signal_processing/energy.c \
	./common_audio/signal_processing/get_scaling_square.c \
	./common_audio/signal_processing/min_max_operations.c \
	./common_audio/signal_processing/real_fft.c \
	./common_audio/signal_processing/spl_init.c \
	./common_audio/signal_processing/vector_scaling_operations.c \
	./common_audio/third_party/fft4g/fft4g.c \
	./common_audio/third_party/spl_sqrt_floor/spl_sqrt_floor.c \
	./modules/audio_processing/legacy_ns/noise_suppression.c \
	./modules/audio_processing/legacy_ns/noise_suppression_x.c \
	./modules/audio_processing/legacy_ns/ns_core.c \
	./modules/audio_processing/legacy_ns/nsx_core.c \
	./modules/audio_processing/legacy_ns/nsx_core_c.c
SRCS_CPP:=\
	./common_audio/signal_processing/dot_product_with_scale.cc \
	./rtc_base/checks.cc
	# ./demo/test.c 

INC:=
INC+=. \
	 ./common_audio/signal_processing/include \
	 ./common_audio/third_party/spl_sqrt_floor \
	 ./rtc_base/numerics \
	 ./rtc_base \
	 ./modules/audio_processing/legacy_ns


SRCS = $(SRCS_C) $(SRCS_CPP)

INC_PATH := $(foreach inc,$(filter-out -I%,$(INC)),$(addprefix -I ,$(inc))) \
			$(filter -I%,$(INC))

OBJS_C := ${patsubst %.c, %.o, $(SRCS_C)}
OBJS_CPP := ${patsubst %.cc, %.o, $(SRCS_CPP)}
OBJS := $(OBJS_C) $(OBJS_CPP)


CFLAGS = -Wall -g  -fPIC $(INC_PATH) -std=c++14 -fno-stack-protector
LDFLAGS = -lm
SHARE = -shared
TARGET_LIB := libAns.a
TARGET_SO := libAns.so


TGT :=
TGT +=$(TARGET_LIB)
TGT +=$(TARGET_SO)
all:$(TGT)

$(OBJS_C):%.o:%.c
	@echo $(BASE_PATH)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJS_CPP):%.o:%.cc
	@echo $(BASE_PATH)
	$(CC) $(CFLAGS) -c $^ -o $@

$(TARGET_LIB):$(OBJS)
	$(AR) rcs  $@ $^

$(TARGET_SO):$(OBJS)
	$(LD) -o $@ $^ $(SHARE) $(LDFLAGS)

clean:
	rm -f $(OBJS_CPP) $(OBJS_C) $(TARGET_LIB) $(TARGET_SO) 
