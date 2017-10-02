#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF 1024

int main() {
    // FIFO file paths
    char *p1 = "/tmp/p1";
    char *p2 = "/tmp/p2";
    char *p3 = "/tmp/p3";
    char *p4 = "/tmp/p4";
    char *p5 = "/tmp/p5";

    // Create the pipes
    mkfifo(p1, 0666);
    mkfifo(p2, 0666);
    mkfifo(p3, 0666);
    mkfifo(p4, 0666);
    mkfifo(p5, 0666);

    int i;
    int fd5 = open(p5, O_RDONLY);
    char buf[MAX_BUF];

    // Wait for p3 to signal its at the end
    while ((i = read(fd5, buf, MAX_BUF)) != 0) {
        buf[i] = '\0';
        if (strcmp("*#*#", buf) == 0) { break; }
    }
    // Close open pipes & Unlink from OS
    close(fd5);

    unlink(p1);
    unlink(p2);
    unlink(p3);
    unlink(p4);
    unlink(p5);

    return 0;
}