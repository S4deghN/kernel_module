export CROSS_COMPILE ?= x86_64-linux-gnu-

obj-m += hello_world.o
hello_world-y := src/main.o

KDIR := ../linux-5.4.281/

BUILD_DIR := $(PWD)/build
BUILD_DIR_MAKEFILE := $(BUILD_DIR)/Makefile

BUILD := make -C $(KDIR) M=$(BUILD_DIR) src=$(PWD)

default: $(BUILD_DIR_MAKEFILE)
	$(BUILD) modules

clean:
	$(BUILD) clean

clangd: clean
	bear --output build/compile_commands.json -- $(BUILD) modules

# insert: default
# 	sudo rmmod hello_world
# 	sudo insmod build/hello_world.ko

$(BUILD_DIR)/src:
	mkdir -p "$@"

$(BUILD_DIR_MAKEFILE): $(BUILD_DIR)/src
	touch "$@"
