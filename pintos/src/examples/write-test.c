#include <stdio.h>
#include "syscall.h"

int
main(void)
{
    int fd = open("xyz");
    if (fd == -1) {
        printf("Failed to open xyz\n");
        return 1;
    }

    int written = write(fd, "Test", 4);
    printf("Write returned: %d\n", written);

    close(fd);
    return 0;
}
