#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF 1024

int main(int argc, const char *argv[]) {
    int i, fd1, fd2;
    char buf[MAX_BUF], ack[MAX_BUF];
    char *p1 = "/tmp/p1";
    char *p2 = "/tmp/p2";

    // Wait until Pipe #1 is open
    while ((fd1 = open(p1, O_WRONLY)) < 0) {}
    while ((fd2 = open(p2, O_RDONLY)) < 0) {}
    
    // Read in input & Send to Process 2
    while (fgets(buf, MAX_BUF, stdin)) {
        if (strcmp("*#*#\n", buf) == 0) { break; }
        // Send to Process 2
        write(fd1, buf, strlen(buf));

        // Wait for acknowledgement
        while((i = read(fd2, ack, MAX_BUF)) != 0) {
            ack[i] = '\0';
            if (strcmp("!", ack) == 0) { 
                printf("Got ack!\n");
                break; 
            }
        }
    }

    // Send finished signal to P2
    write(fd1, buf, strlen(buf));
    close(fd1);

    return 0;
}