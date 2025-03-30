CROSS_COMPILE = riscv64-unknown-linux-gnu-
CFLAGS = -nostdlib -fno-builtin -march=rv32g -mabi=ilp32 -g -Wall

QEMU = qemu-system-riscv32
QFLAGS = -nographic -smp 1 -machine virt -bios none

GDB = riscv64-unknown-linux-gnu-gdb
CC = ${CROSS_COMPILE}gcc
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump
