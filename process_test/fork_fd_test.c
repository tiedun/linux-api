#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char const *argv[])
{ 
    // 如果文件不存在，创建时必须通过第三个参数指定文件权限
    int fd = open("io.txt", O_APPEND | O_CREAT | O_WRONLY, 0644);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    char buffer[1024]; // 创建一个缓冲区来存放读取的数据
    pid_t pid;
    
    if ((pid = fork()) == 0) {
        strcpy(buffer, "这是子进程写入的数据！\n");
    } else if (pid > 0) {
        
        sleep(1);
        printf("父进程计时结束\n");
        strcpy(buffer, "这是父进程写入的数据！\n");
    } else {
        printf("子进程创建失败\n");
        return -1;
    }

    ssize_t bytes_write;

    bytes_write = write(fd, buffer, strlen(buffer));
    if (bytes_write == -1) {
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd); // 使用完毕后关闭文件描述符

    if (pid == 0) {
        printf("子进程写入完毕，并释放文件描述符\n");
    } else if (pid > 0) {
        printf("父进程写入完毕，并释放文件描述符\n");
    }

    return 0;
}
