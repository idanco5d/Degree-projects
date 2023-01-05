.globl my_ili_handler
.extern what_to_do, old_ili_handler

.text
.align 4, 0x90

my_ili_handler:
  ####### Some smart student's code here #######
  pushq %rax
  pushq %rbx
  pushq %rcx
  pushq %rdx	
  pushq %r8
  pushq %r9
  pushq %r10
  pushq %r11
  pushq %r12
  pushq %r13
  pushq %r14
  pushq %r15
  pushq %rsi
  pushq %rbp
  pushq %rsp

  xorq %rdi, %rdi
  xorq %rax, %rax
  xorq %r12, %r12

  movq 120(%rsp), %r12
  movq (%r12), %r12
  cmpb $0x0f, %r12b
  je two_bytes
# one_byte
  movb %r12b, %dil
  jmp call_what_to_do

two_bytes:
  shr $8, %r12
  movb %r12b, %dil

call_what_to_do:
  call what_to_do
  testq %rax, %rax
  je call_old_ili_handler
# our_handler
  movq %rax, %rdi
  movq 120(%rsp), %r12
  addq $2, %r12
  movq %r12, 120(%rsp)
  popq %rsp
  popq %rbp
  popq %rsi
  popq %r15
  popq %r14
  popq %r13
  popq %r12
  popq %r11
  popq %r10
  popq %r9
  popq %r8
  popq %rdx
  popq %rcx
  popq %rbx
  popq %rax
  
  jmp end
call_old_ili_handler:
  popq %rsp
  popq %rbp
  popq %rsi
  popq %r15
  popq %r14
  popq %r13
  popq %r12
  popq %r11
  popq %r10
  popq %r9
  popq %r8
  popq %rdx
  popq %rcx
  popq %rbx
  popq %rax
  
  jmp * old_ili_handler

  end: 
 
  iretq
