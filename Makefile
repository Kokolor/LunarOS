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
	cp background.bmp iso_root/
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
	rm fat32.img
	rm *.o

fat32_image:
	dd if=/dev/zero of=fat32.img bs=1M count=64
	mkfs.fat -F32 fat32.img

run: fat32_image
	make iso
	qemu-system-x86_64.exe -L "C:/Program Files/qemu" -m 4096M -debugcon stdio -hda fat32.img -cdrom lunar.iso -boot d