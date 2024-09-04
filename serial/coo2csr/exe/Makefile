# makefile for app_exe
# module load compilers/gcc/10.3.0 generators/make/4.3/gcc-10.3.0
CC = gcc
CFLAGS = -Wall -g -O3
SRC = ../src
OBJECT = $(patsubst %.c,%.o,$(wildcard $(SRC)/*.c))
# LIB = -lm
LIB = 

# 默认设置为空
type_real :=
type_complex :=

# 根据命令行目标设置相应的变量
ifeq ($(filter type_real, $(MAKECMDGOALS)),type_real)
type_real = 1
endif

ifeq ($(filter type_complex, $(MAKECMDGOALS)),type_complex)
type_complex = 1
endif

# 伪造目标以避免误解释
type_real: 
	@:

type_complex:
	@:

.PHONY: all clean

all: check_config app_exe

check_config:
	@if [ "$(type_real)$(type_complex)" = "" ]; then \
	    echo "You must set exactly one of type_real or type_complex to 1"; \
	    exit 1; \
	elif [ "$(type_real)$(type_complex)" = "11" ]; then \
	    echo "You cannot set both type_real and type_complex to 1 at the same time"; \
	    exit 1; \
	fi

app_exe: $(OBJECT)
	$(CC) $(CFLAGS) $(CFLAGS_ADD) -o app_exe $(OBJECT) $(LIB)

%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_ADD) -c "$<" -o "$@"

clean:
	rm -f app_exe $(OBJECT)

# 将 CFLAGS_ADD 设置为相应的标志
ifeq ($(type_real),1)
CFLAGS_ADD = -DTYPE_REAL
endif

ifeq ($(type_complex),1)
CFLAGS_ADD = -DTYPE_COMPLEX
endif

# command line
# 'make type_real all' for real version
# 'make type_complex all' for complex version
# 'make clean' clean all objects