# Makefile template for compiling audio_gadget.c
ifeq ($(CROSS_COMPILE),)
CROSS_COMPILE = /opt/arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-
endif

# Compiler and flags
CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra -O2

# Source file and executable name
SRC = audio_gadget.c
EXEC = audio_gadget

HR_LOCAL_DIR ?= $(realpath ../../..)
SYSROOT_DIR ?= $(HR_LOCAL_DIR)/deploy/rootfs
SYSROOT_LIB = $(SYSROOT_DIR)/usr/lib/aarch64-linux-gnu

INCLUDE_DIR = -I../include/iar/ -I../include/vio/
SYSROOT_FLAGS = --sysroot=$(SYSROOT_DIR) -B$(SYSROOT_LIB) \
	-isystem $(SYSROOT_DIR)/usr/include/aarch64-linux-gnu \
	-L$(SYSROOT_LIB) -Wl,-rpath-link=$(SYSROOT_LIB)
LIB_DIR = -L../lib/ -L../lib/hbmedia -L../lib/aarch64-linux-gnu -Wl,-rpath-link=../lib/ -L./

# Libraries to link
LIBS = -lasound -lm

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $(SYSROOT_FLAGS) -o $@ $^ $(INCLUDE_DIR) $(LIB_DIR) $(LIBS)

clean:
	rm -f $(EXEC)

.PHONY: all clean
