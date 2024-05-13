#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int pid, subprocess_status;
    char *cmd = "/usr/bin/ping";
    char *argv[] = {cmd, "-c", "10", "www.atguigu.com", NULL};
    char *envi[] = {NULL};
    pid = fork();
    if (pid < 0)
    {
        printf("新学员邀请失败!\n");
    }
    else if (pid == 0)
    {
        // 启动一个ping10次的子进程
        printf("新学员联系10次海哥\n");
        execve(cmd, argv, envi);
    }
    else
    {
        printf("老学员%d继续学习等待新学员%d联系完成!\n",getpid(), pid);
        // 指定需要等待的子进程号
        waitpid(pid, &subprocess_status, 0);
    }
    printf("老学员等待新学员联系完成\n");
    return 0;
}
