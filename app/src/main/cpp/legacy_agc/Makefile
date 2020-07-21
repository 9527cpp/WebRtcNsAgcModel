#COMPILIE_PREFIX = arm-openwrt-linux-muslgnueabi-
CC = $(COMPILIE_PREFIX)g++
AR = $(COMPILIE_PREFIX)ar

BASE_PATH=$(PWD)


# SRCS += ${wildcard *.c}
SRCS:=
SRCS_C:= \
	./common_audio/signal_processing/copy_set_operations.c \
	./common_audio/signal_processing/division_operations.c \
	./common_audio/signal_processing/resample_by_2.c \
	./common_audio/signal_processing/spl_sqrt.c \
	./common_audio/third_party/spl_sqrt_floor/spl_sqrt_floor.c \
	./modules/audio_processing/agc/legacy/analog_agc.c \
	./modules/audio_processing/agc/legacy/digital_agc.c

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
	 ./modules/audio_processing/agc/legacy


SRCS = $(SRCS_C) $(SRCS_CPP)

INC_PATH := $(foreach inc,$(filter-out -I%,$(INC)),$(addprefix -I ,$(inc))) \
			$(filter -I%,$(INC))

OBJS_C := ${patsubst %.c, %.o, $(SRCS_C)}
OBJS_CPP := ${patsubst %.cc, %.o, $(SRCS_CPP)}
OBJS := $(OBJS_C) $(OBJS_CPP)


CFLAGS = -Wall -g  -fPIC $(INC_PATH) -std=c++14
LDFLAGS = 
TARGET := libAgc.a

all:$(TARGET)

$(OBJS_C):%.o:%.c
	@echo $(BASE_PATH)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJS_CPP):%.o:%.cc
	@echo $(BASE_PATH)
	$(CC) $(CFLAGS) -c $^ -o $@

$(TARGET):$(OBJS)
	$(AR) cq $@ $^

clean:
	rm -f $(OBJS_CPP) $(OBJS_C) $(TARGET) 
