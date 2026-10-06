  # only using conditional jumps + single cmp
  .globl  find_range
find_range:
  vxorps %xmm1, %xmm1, %xmm1
  ucomiss %xmm1, %xmm0
  jp .L0
  jb .L1
  je .L2
  ja .L3
.L0:
  movq $3, %rax
  jmp .L4
.L1:
  xorq %rax, %rax
  jmp .L4
.L2:
  movq $1, %rax
  jmp .L4
.L3:
  movq $2, %rax
.L4:
  ret

  # only using conditional moves + single cmp
  .globl  find_range_2
find_range_2:
  vxorps %xmm1, %xmm1, %xmm1
  ucomiss %xmm1, %xmm0
  movq $2, %rax
  movq $1, %rdx
  cmove %rdx, %rax
  movq $0, %rdx
  cmovb %rdx, %rax
  movq $3, %rdx
  cmovp %rdx, %rax
  ret
