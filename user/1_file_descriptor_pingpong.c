#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) 
{
    int fd[2];
    char byte = 'a';
    pipe(fd);

    if (fork() == 0) {
        read(fd[0], &byte, 1);
        close(fd[0]);
        printf("%d: received ping\n", getpid());

        write(fd[1], &byte, 1);
        close(fd[1]);
        exit(0);
    } else {
        write(fd[1], &byte, 1);
        close(fd[1]);

        read(fd[0], &byte, 1);
        close(fd[0]);

        printf("%d: received pong\n", getpid());
        exit(0);
    }
}