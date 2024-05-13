#include <stdio.h>
#include <unistd.h>

int main() {

    int val = 123;

    // 定义变量接收子进程PID
    __pid_t pid;
    // 创建一个子进程
    if ((pid = fork()) > 0 ){
        sleep(1);
        printf("父进程中val 的内容是: %d\nval 所在的地址是: %p\n", val, &val);
    } else if (pid == 0) {
        val = 321;
        printf("子进程中val 的内容是: %d\nval 所在的地址是: %p\n", val, &val);
    } else {
        printf("子进程创建失败\n");
    }
    
    return 0;
}
