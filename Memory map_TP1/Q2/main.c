#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {

    int fd;
    fd = open("test.txt", O_RDONLY);
    printf("%d\n", fd);
    return 0;
}

