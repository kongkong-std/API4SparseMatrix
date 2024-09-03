# makefile for app_exe
# module load compilers/gcc/10.3.0 generators/make/4.3/gcc-10.3.0
CC = gcc
CFLAGS = -Wall -g -O3
SRC = ../src
OBJECT = $(patsubst %.c,%.o,$(wildcard $(SRC)/*.c))
# LIB = -lm
LIB = 

# 检查是否未设置或同时设置了 type_real 和 type_complex
ifeq ($(type_real)$(type_complex),)
$(error You must set either type_real or type_complex)
endif

ifeq ($(type_real)$(type_complex),11)
$(error You cannot set both type_real and type_complex at the same time)
endif

# 如果 type_real 被设置，则添加 -DTYPE_REAL
ifdef type_real
CFLAGS += -DTYPE_REAL
endif

# 如果 type_complex 被设置，则添加 -DTYPE_COMPLEX
ifdef type_complex
CFLAGS += -DTYPE_COMPLEX
endif

.PHONY: all clean
all: app_exe

app_exe: $(OBJECT)
	$(CC) $(CFLAGS) -o app_exe $(OBJECT) $(LIB)

%.o: %.c
	$(CC) $(CFLAGS) -c "$<" -o "$@"

clean:
	rm -f app_exe $(OBJECT)