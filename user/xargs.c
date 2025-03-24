#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: xargs command\n");
        exit(1);
    }

    char* args[argc + 1];
    for (int i = 1; i < argc; i++) {
        args[i - 1] = (char*)malloc(MAXARG);
        strcpy(args[i - 1], argv[i]);
    }
    
    char tmp;
    /* 如果先让p指向未赋值的args[]则p会为null从而无法运行 *
     * 注意每一项的实质是字符指针 */
    args[argc - 1] = (char*)malloc(MAXARG);
    char* p = args[argc - 1];
    args[argc] = 0;
    while (read(STDIN_FILENO, &tmp, 1) > 0) {
        if (tmp == '\n') {
            if (fork() == 0) {
                *p = '\0';
                exec(args[0], args);
                exit(1);
            } else {
                wait(0);
                p = args[argc - 1];
            }
        } else {
            *p++ = tmp;
        }
    }
    
    free(args[argc - 1]);
    for (int i = 1; i < argc; i++) {
        free(args[i - 1]);
    }
    
    exit(0);
}

//对字符串操作常用一个指针p指向字符串的末尾