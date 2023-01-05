.global count_above

.section .data
num: .zero 20
char: .byte 0

# rax, rdi, rsi, rdx, rcx, r8, r9, r10, r11
# r10 = *file
# r12b = seperator
# r13 = limit
# r9b = count
# dil = char
# r8 = *num
# r14b = EOF_flag

.section .text
count_above:
  push %r12
  push %r13
  push %r14
  xorq %r14, %r14
  xorq %r12, %r12
  movb %dil, %r12b
  movq %rsi, %r13
  call get_path
  movq %rax, %rdi
# open file
  movq $0, %rsi
  movq $0, %rdx
  movq $0x2, %rax
  syscall
  movq %rax, %r10
  xorq %r9, %r9
EOF_loop:
  leaq num(%rip), %r8
string_loop:
# char = *file
  movq $0, %rax
  movq %r10, %rdi
# read file
  leaq char(%rip), %rsi
  movq $1, %rdx
  syscall
# i.f rax equal 0 to EOF
  cmp $0, %rax
  je EOF_flag_setter

# i.f char equal seperator
  movb char(%rip), %dil
  cmp %dil, %r12b
  je string_loop_end
  movb %dil, (%r8)
  inc %r8
  jmp string_loop
# out of string_Loop
EOF_flag_setter:
  movb $1 , %r14b
string_loop_end:
  movb $0, (%r8)
  lea num(%rip), %rdi
# precall
  push %r10
  push %r9
  call atam_atol
  pop %r9
  pop %r10
# i.f rax greater limit then i.f true then count++
  cmp %r13, %rax
  jle check_EOF
  inc %r9
check_EOF:
  test %r14b, %r14b
  je EOF_loop
# end of EOF_Loop
# close the file with syscall
  movq $3, %rax
  movq %r10, %rdi
  syscall
  mov %r9, %rax
  pop %r14
  pop %r13
  pop %r12
  ret

