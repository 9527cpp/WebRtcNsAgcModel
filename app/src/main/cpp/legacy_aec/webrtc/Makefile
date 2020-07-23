#COMPILIE_PREFIX = arm-openwrt-linux-muslgnueabi-
CC = $(COMPILIE_PREFIX)g++
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
	./common_audio/signal_processing/randomization_functions.c \
	./common_audio/signal_processing/real_fft.c \
	./common_audio/signal_processing/spl_init.c \
	./common_audio/signal_processing/vector_scaling_operations.c \
	./common_audio/third_party/fft4g/fft4g.c \
	./common_audio/third_party/spl_sqrt_floor/spl_sqrt_floor.c \
	./modules/audio_processing/aec/aec_core.c \
	./modules/audio_processing/aec/aec_core_sse2.c \
	./modules/audio_processing/aec/aec_rdft.c \
	./modules/audio_processing/aec/aec_rdft_sse2.c \
	./modules/audio_processing/aec/aec_resampler.c \
	./modules/audio_processing/aec/echo_cancellation.c \
	./modules/audio_processing/utility/delay_estimator.c \
	./modules/audio_processing/utility/delay_estimator_wrapper.c \
	./modules/audio_processing/utility/fft4g.c \
	./modules/audio_processing/utility/ring_buffer.c
SRCS_CPP:=\
	./common_audio/signal_processing/dot_product_with_scale.cc \
	./rtc_base/checks.cc
	# ./demo/test.c 

INC:=
INC+=. \
	 ../ \
	 ./common_audio/signal_processing/include \
	 ./common_audio/third_party/spl_sqrt_floor \
	 ./rtc_base/numerics \
	 ./rtc_base \
	 ./modules/audio_processing/aec


SRCS = $(SRCS_C) $(SRCS_CPP)

INC_PATH := $(foreach inc,$(filter-out -I%,$(INC)),$(addprefix -I ,$(inc))) \
			$(filter -I%,$(INC))

OBJS_C := ${patsubst %.c, %.o, $(SRCS_C)}
OBJS_CPP := ${patsubst %.cc, %.o, $(SRCS_CPP)}
OBJS := $(OBJS_C) $(OBJS_CPP)


CFLAGS = -Wall -g  -fPIC $(INC_PATH) -std=c++14
LDFLAGS = -lm
SHARE = -shared
TARGET_LIB := libAec.a
TARGET_SO := libAec.so


TGT :=
TGT +=$(TARGET_LIB)
# TGT +=$(TARGET_SO)
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
	$(CC) -o $@ $^ $(SHARED) $(LDFLAGS)

clean:
	rm -f $(OBJS_CPP) $(OBJS_C) $(TARGET_LIB) $(TARGET_SO) 
