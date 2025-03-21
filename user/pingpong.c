#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) 
{
    int parent_fd[2], child_fd[2];
    char byte = 'a';
    pipe(parent_fd);
    pipe(child_fd);

    if (fork() == 0) {
        close(parent_fd[1]);
        close(child_fd[0]);

        read(parent_fd[0], &byte, 1);
        close(parent_fd[0]);
        printf("%d: received ping\n", getpid());

        write(child_fd[1], &byte, 1);
        close(child_fd[1]);
        exit(0);
    } else {
        close(parent_fd[0]);
        close(child_fd[1]);

        write(parent_fd[1], &byte, 1);
        close(parent_fd[1]);

        read(child_fd[0], &byte, 1);
        close(child_fd[0]);

        printf("%d: received pong\n", getpid());
        exit(0);
    }
}