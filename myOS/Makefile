include common.mk

vpath %.S asm/
vpath %.S startup/
vpath %.c kernel/
vpath %.c lib/

SYSCALL = y
ifeq (${SYSCALL}, y)
CFLAGS += -D CONFIG_SYSCALL
endif

SRCS_ASM = \
	start.S \
	mem.S \
	entry.S \
	sys.S \
	usys.S \

SRCS_C = \
	kernel.c \
	uart.c \
	printf.c \
	page.c \
	sched.c \
	user.c \
	heap.c \
	trap.c \
	plic.c \
	timer.c \
	lock.c \
	syscall.c \


OBJS = $(SRCS_ASM:.S=.o)
OBJS += $(SRCS_C:.c=.o)

.DEFAULT_GOAL := all
all: os.elf

# start.o must be the first in dependency!
os.elf: ${OBJS}
	${CC} ${CFLAGS} -T os.ld -o os.elf $(addprefix objs/,$^)
	${OBJCOPY} -O binary os.elf os.bin

%.o : %.c
	if [ ! -d "objs" ]; then mkdir objs; fi
	${CC} ${CFLAGS} -c -I lib/ -I kernel/ -o $(addprefix objs/,$@) $<

%.o : %.S
	if [ ! -d "objs" ]; then mkdir objs; fi
	${CC} ${CFLAGS} -c -I lib/ -I kernel/ -o $(addprefix objs/,$@) $<

run: all
	@${QEMU} -M ? | grep virt >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo "------------------------------------"
	@${QEMU} ${QFLAGS} -kernel os.elf

.PHONY : debug
debug: all
	@echo "Press Ctrl-C and then input 'quit' to exit GDB and QEMU"
	@echo "-------------------------------------------------------"
	@${QEMU} ${QFLAGS} -kernel os.elf -s -S &
	@${GDB} os.elf -q -x ./gdbinit

.PHONY : code
code: all
	@${OBJDUMP} -S os.elf | less

.PHONY : clean
clean:
	rm -rf objs/*.o *.bin *.elf

