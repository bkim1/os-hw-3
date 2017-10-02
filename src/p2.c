#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF 1024

int main(int argc, const char *argv[]) {
    int i, d, fd1, fd2, fd3, fd4;
    char buf[MAX_BUF], ack[MAX_BUF];
    char *p1 = "/tmp/p1";
    char *p2 = "/tmp/p2";
    char *p3 = "/tmp/p3";
    char *p4 = "/tmp/p4";

    // Wait until Pipe #1, #2, #3, #4 are open
    while ((fd1 = open(p1, O_RDONLY)) < 0) {} // Read from P1
    while ((fd2 = open(p2, O_WRONLY)) < 0) {} // Write to P1
    while ((fd3 = open(p3, O_WRONLY)) < 0) {} // Write to P3
    while ((fd4 = open(p4, O_RDONLY)) < 0) {} // Read from P3
    
    while ((i = read(fd1, buf, MAX_BUF)) != 0) {
        buf[i] = '\0';

        // Check for user input to end
        if (strcmp("*#*#\n", buf) == 0) { break; }

        // Change case of each character
        int c;
        for (c = 0; c < i; c++) {
            if (buf[c] <= 'Z' && buf[c] >= 'A') { buf[c] = buf[c] + 32; }
            else if (buf[c] <= 'z' && buf[c] >= 'a') { buf[c] = buf[c] - 32; }
        }
        printf("%s", buf);

        // Write to Process 3
        write(fd3, buf, strlen(buf));

        // Send Acknowledgement to Process 1
        write(fd2, "!", strlen("!"));

        // Wait for acknowledgement from Process 3
        while((d = read(fd4, ack, MAX_BUF)) != 0) {
            ack[i] = '\0';
            if (strcmp("!", ack) == 0) { break; }
        }
    }

    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);

    return 0;
}