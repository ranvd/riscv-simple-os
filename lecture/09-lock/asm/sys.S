.globl atomic_swap
.align 4
atomic_swap:
        li a5, 1
        amoswap.w.aq a5, a5, 0(a0) // rd, rs2, rs1; periperally execute rd=rs1 and rs1=rs2.
        mv a0, a5
        ret