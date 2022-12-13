#include "platform.h"

    # declare an variable which is used for the size of hart's stack size
    .equ    STACK_SIZE, 1024

    .global _start

    .text
_start:
    # park hards if the hart_id != 0
    csrr    t0, mhartid     # read current hear id
    mv  tp, t0      # keep CPU's hartid in its $tp for later used
    bnez    t0, park        # if the hart_id != 0, park it.

    # Set all bytes in the BSS section to zero
    # 其實我不知道為什麼要特別去將 bss section 清成 0，但教學上就是這樣做了。
    la  a0, _bss_start
    la  a1, _bss_end
    bgeu    a0, a1, 2f
1:
    sw  zero, (a0) 
    addi    a0, a0, 4
    bltu    a0, a1, 1b
2:
    # Setup stacks, the stack grows from bottom to top, so we put the
	# stack pointer to the very end of the stack range.
    slli    t0, t0, 10      # shift left logic by 1024
    la  sp, stacks + STACK_SIZE     # set the initial stack pointer
					# to the end of the first stack space
    add sp, sp, t0      # move the offset by hart_id

#ifdef CONFIG_SYSCALL
	# https://lore.kernel.org/qemu-devel/20201223192553.332508-1-atish.patra@wdc.com/
	# For qemu version >= 6.0, exception would be raised if no PMP enty is
	# configured. So just configure one entny, which allows all the whole
	# 32-bits physical address range is R/W/X.
	# FIXME: I say it is a temporary workaroud due to I think the patch
	# above contains bug, and I have raised new issue to qemu but it has not
	# been rootcaused till now. Details please refer to
	# https://gitlab.com/qemu-project/qemu/-/issues/585 or
	# https://gitee.com/unicornx/riscv-operating-system-mooc/issues/I441IC (in chinese)
	# So it's just a temporary workaround till now to not block people who
	# want to try newer qemu (>= 6.0).
	li      t0, 0xffffffff
	csrw    pmpaddr0, t0
	li      t0, 0xf
	csrw    pmpcfg0, t0
#endif

	# At the end of start_kernel, schedule() will call MRET to switch
	# to the first task, so we parepare the mstatus here.
	# Notice: default mstatus is 0
#ifdef CONFIG_SYSCALL
	# Keep mstatus.MPP as 0, so we will run in User mode after MRET.
	# Set mstatus.MPIE to 1, so MRET will enable the interrupt.
	li	t0, 1 << 7
#else
	# Set mstatus.MPP to 3, so we still run in Machine mode after MRET.
	# Set mstatus.MPIE to 1, so MRET will enable the interrupt.
	li	t0, 3 << 11 | 1 << 7
#endif
	csrr	a1, mstatus
	or	t0, t0, a1
	csrw	mstatus, t0

    j   start_kernel    # hart 0 jump to c


park:
    wfi
    j park  # jump to park

stacks:
    .skip   STACK_SIZE * MAXNUM_CPU     # allocate space for all the harts stack

    