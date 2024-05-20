#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main() {
    FILE *file = fopen("/home/atguigu/socket_test/testfile.txt", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    // 设置文件为全缓冲模式
    if (setvbuf(file, NULL, _IOFBF, 0) != 0) {
        perror("Failed to set buffer mode");
        return 1;
    }

    fprintf(file, "hello\n");

    fflush(file);

    char *argv[] = {"/usr/bin/ping", "-c", "1", "www.baidu.com", NULL};
    char *envp[] = {NULL};

    execve("/usr/bin/ping", argv, envp);
    perror("execve");

    return 0;
}
