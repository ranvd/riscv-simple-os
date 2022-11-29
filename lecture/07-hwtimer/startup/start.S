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

    j   start_kernel    # hart 0 jump to c


park:
    wfi
    j park  # jump to park

stacks:
    .skip   STACK_SIZE * MAXNUM_CPU     # allocate space for all the harts stack

    