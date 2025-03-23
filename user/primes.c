#include "kernel/types.h"
#include "user/user.h"

void seive(int* parent_fd) {
    if(fork() == 0) { // 读滤写
        int prime;
        if(read(parent_fd[0], &prime, sizeof(prime)) == 0) {
            close(parent_fd[0]);
            exit(0);
        }
        printf("prime %d\n", prime);
        
        int child_fd[2];
        pipe(child_fd);
        int tmp;
        while(read(parent_fd[0], &tmp, sizeof(tmp)) != 0) {
            if(tmp % prime != 0) {
                write(child_fd[1], &tmp, sizeof(tmp));
            }
        }
        close(parent_fd[0]);
        close(child_fd[1]);
        seive(child_fd);
    } else { // 等待子进程结束
        close(parent_fd[0]);
        wait(0);
    }
}

int main() {
    int parent_fd[2];
    pipe(parent_fd);
    for(int i=2; i<=35; i++) {
        write(parent_fd[1], &i, sizeof(i));
    }
    close(parent_fd[1]);
    seive(parent_fd);
    exit(0);
}