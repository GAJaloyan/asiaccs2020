all: static

static: static.c
	riscv64-unknown-elf-gcc -o static.out -static -fstack-protector-strong static.c
