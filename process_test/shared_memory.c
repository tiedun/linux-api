#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>

int main()
{
    char *share;
    pid_t pid;
    char shmName[100]={0};
    sprintf(shmName,"/letter%d",getpid());
    // 共享内存对象的文件标识符
    int fd;
    fd = shm_open(shmName, O_CREAT | O_RDWR, 0644);
    if (fd < 0)
    {
        perror("共享内存对象开启失败!\n");
        exit(EXIT_FAILURE);
    }
    // 将该区域扩充为100字节长度
    ftruncate(fd, 100);
    // 以读写方式映射该区域到内存，并开启父子共享标签 偏移量选择0从头开始
    share = mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    // 注意:不是p == NULL 映射失败返回的是((void *) -1)
    if (share == MAP_FAILED)
    { 
        perror("共享内存对象映射到内存失败!\n");
        exit(EXIT_FAILURE);
    }
    // 映射区建立完毕,关闭读取连接 注意不是删除
    close(fd);
    // 创建子进程
    pid = fork(); 
    if (pid == 0)
    {
        // 子进程写入数据作为回信 
        strcpy(share, "你是个好人!\n");
        printf("新学员%d完成回信!\n", getpid());
    }
    else
    {
        // 等待回信
        sleep(1);
        printf("老学员%d看到新学员%d回信的内容: %s", getpid(),pid,share);
        // 等到子进程运行结束
        wait(NULL);
        // 释放映射区
        int ret = munmap(share, 100); 
        if (ret == -1)
        {
            perror("munmap");
            exit(EXIT_FAILURE);
        }
    }
    // 删除共享内存对象
    shm_unlink(shmName);
    return 0;
}
