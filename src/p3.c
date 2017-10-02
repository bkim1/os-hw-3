#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF 1024

int main(int argc, const char *argv[]) {
    int i, fd3, fd4, fd5;
    char buf[MAX_BUF];
    char *p3 = "/tmp/p3";
    char *p4 = "/tmp/p4";
    char *p5 = "/tmp/p5";


    // Wait until Pipe #2 & #3 are open
    while ((fd3 = open(p3, O_RDONLY)) < 0) {}
    while ((fd4 = open(p4, O_WRONLY)) < 0) {}
    while ((fd5 = open(p5, O_WRONLY)) < 0) {}
    
    // Get stuff from Pipe #2
    while ((i = read(fd3, buf, MAX_BUF)) != 0) {
        buf[i] = '\0';
        write(fd4, "!", strlen("!"));

        if (strcmp("*#*#", buf) == 0) { break; }
        
        printf("%s\n", buf);
    }
    // Notify Process 0 to close the pipes
    write(fd5, "*#*#", strlen("*#*#"));

    close(fd3);
    close(fd4);
    close(fd5);

    return 0;
}