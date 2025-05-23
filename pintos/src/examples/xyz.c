#include <stdio.h>
#include <syscall.h>

int main(void) {
    int fd = open("echo");
    if (fd < 0) {
        printf("open failed\n");
        return -1;
    }
    int n = write(fd, "x", 1);
    printf("wrote: %d\n", n);
    close(fd);
    return 0;
}
