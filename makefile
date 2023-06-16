SRCS := $(shell find kernel/ -name '*.c') 
OBJS := $(SRCS:.c=.o)
CC = gcc
LD = ld

CFLAGS := \
	-Ikernel/src \
	-Wall \
	-Wextra \
	-std=gnu11 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-PIE \
	-w \
	-fno-PIC \
	-m64 \
	-march=x86-64 \
	-mabi=sysv \
	-mno-80387 \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel

LD_FLAGS := \
	-Tkernel/link.ld \
	-nostdlib \
	-static \
	-m elf_x86_64 \
	-z max-page-size=0x1000

.SUFFIXE: .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel.elf: $(OBJS)
	nasm kernel/src/arch/x86_64/gdt.asm -felf64 -o _gdt.o
	$(LD) $(LD_FLAGS) $(OBJS) _gdt.o -o $@

iso:
	rm -rf iso_root
	mkdir -p iso_root
	cp kernel.elf \
		limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-cd-efi.bin iso_root/
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-cd-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso_root -o lunar.iso
	limine/limine-deploy lunar.iso
	rm -rf iso_root

clean:
	rm -f $(OBJS)
	rm -f kernel.elf
	rm -f lunar.iso

run:
	make iso
	qemu-system-x86_64 -m 128M -enable-kvm -debugcon stdio -cdrom ./lunar.iso
