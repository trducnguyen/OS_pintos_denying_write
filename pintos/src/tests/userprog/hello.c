#include <stdio.h>
#include <syscall.h>

int main(void) {
    asm volatile ("movl $1, %eax; int $0x30"); // syscall_number = 1
    return 0;
}
