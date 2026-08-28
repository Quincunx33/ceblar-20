PROJECT := ceblar-20
BUILD_DIR := build
ISO_DIR := $(BUILD_DIR)/isodir
ISO := $(BUILD_DIR)/$(PROJECT).iso
CC := i686-linux-gnu-gcc
LD := i686-linux-gnu-ld
AS := nasm
GRUB_MKRESCUE := grub-mkrescue
QEMU := qemu-system-i386
CFLAGS := -std=gnu11 -ffreestanding -fno-stack-protector -fno-pie -fno-asynchronous-unwind-tables -Wall -Wextra -Werror -m32 -Iinclude
LDFLAGS := -m elf_i386 -T arch/x86/linker.ld
C_SRCS := $(shell find kernel arch drivers mm modules scheduler fs userspace net -name '*.c')
C_OBJS := $(patsubst %.c,$(BUILD_DIR)/c/%.o,$(C_SRCS))
S_SRCS := $(shell find arch -name '*.S')
S_OBJS := $(patsubst %.S,$(BUILD_DIR)/s/%.o,$(S_SRCS))

.PHONY: all iso run check clean help debug size tags test test-smp test-virtio
all: iso

$(BUILD_DIR)/c/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/s/%.o: %.S
	@mkdir -p $(dir $@)
	$(AS) -f elf32 $< -o $@

$(BUILD_DIR)/kernel.elf: $(S_OBJS) $(C_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

iso: $(BUILD_DIR)/kernel.elf
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(BUILD_DIR)/kernel.elf $(ISO_DIR)/boot/kernel.elf
	printf '%s\n' 'set timeout=0' 'set default=0' 'menuentry "ceblar-20" {' '  multiboot2 /boot/kernel.elf' '  boot' '}' > $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB_MKRESCUE) -o $(ISO) $(ISO_DIR) >/dev/null

run: iso
	$(QEMU) -cdrom $(ISO) -serial stdio -display curses

check:
	@printf 'Compiler: '; command -v $(CC) >/dev/null && printf 'available\n' || printf 'missing\n'
	@printf 'Assembler: '; command -v $(AS) >/dev/null && printf 'available\n' || printf 'missing\n'
	@printf 'GRUB ISO tool: '; command -v $(GRUB_MKRESCUE) >/dev/null && printf 'available\n' || printf 'missing\n'
	@printf 'QEMU: '; command -v $(QEMU) >/dev/null && printf 'available\n' || printf 'missing\n'

clean:
	rm -rf $(BUILD_DIR)

debug: CFLAGS += -g -DDEBUG
debug: iso
	@printf '%s\n' 'Debug ISO built: use qemu -s -S and gdb separately.'

size: $(BUILD_DIR)/kernel.elf
	@i686-linux-gnu-size $(BUILD_DIR)/kernel.elf

tags:
	@command -v ctags >/dev/null && ctags -R kernel arch mm drivers fs modules scheduler net include || printf '%s\n' 'ctags not installed'

test: iso
	@tools/qemu_test.sh
test-smp: iso
	@tools/qemu_smp_test.sh

test-virtio: iso
	@tools/qemu_virtio_test.sh

help:
	@printf '%s\n' 'Targets: all, iso, run, check, clean, debug, size, tags, test, test-smp, test-virtio'
