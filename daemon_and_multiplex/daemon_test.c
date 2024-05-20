#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <syslog.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

void signal_handler(int sig)
{
    switch (sig)
    {
    case SIGHUP:
        // Handle hang-up signal
        syslog(LOG_WARNING, "Received SIGHUP signal.");
        break;
    case SIGTERM:
    case SIGKILL:
        // Handle termination signal
        syslog(LOG_INFO, "Daemon exiting");
        exit(0);
        break;
    default:
        syslog(LOG_INFO, "Received unhandled signal");
    }
}

void daemonize()
{
   pid_t pid;

    // Fork off the parent process
    pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0)
        exit(EXIT_SUCCESS);

    if (setsid() < 0)
        exit(EXIT_FAILURE);

    // 处理 SIGHUP、SIGTERM、SIGKILL 信号
    signal(SIGHUP, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGKILL, signal_handler);

    pid = fork();

    if (pid < 0)
        exit(EXIT_FAILURE);

    if (pid > 0)
        exit(EXIT_SUCCESS);

    // 重置umask
    umask(0);

    // 将工作目录切换为根目录
    chdir("/");

    // 关闭所有打开的文件描述符
    for (int x = 0; x <= sysconf(_SC_OPEN_MAX); x++)
    {
        close(x);
    }

    int fd0 = open("/dev/null", O_RDWR);
int fd1 = dup(0);
int fd2 = dup(0);

if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
    // 如果文件描述符不是预期的（即 0, 1, 2），则有错误处理。
    syslog(LOG_ERR, "Unsuccessful in redirecting file descriptors to /dev/null");
    exit(EXIT_FAILURE);
}
/*    openlog("this is our daemonize process: ", LOG_PID, LOG_DAEMON);*/
}

int main()
{
    daemonize();

    while (1)
    {
        pid_t pid = fork();

        if (pid > 0)
        {
            syslog(LOG_INFO, "守护进程正在监听服务端进程...");
            waitpid(-1, NULL, 0);
            syslog(LOG_ERR, "服务端进程终止，3s后重启...");
            sleep(3);
        }
        else if (pid == 0)
        {
            syslog(LOG_INFO, "子进程fork成功");
            syslog(LOG_INFO, "启动服务端进程");

            char *path = "/home/atguigu/daemon_and_multiplex/tcp_server";
            char *argv[] = {"my_tcp_server", NULL};
            errno = 0;
            execve(path, argv, NULL);

            char buf[1024];
            sprintf(buf, "errno: %d", errno);
            syslog(LOG_ERR, "%s", buf);

            // perror
            syslog(LOG_ERR, "服务端进程启动失败");
            exit(EXIT_FAILURE);
        }
        else
        {
            syslog(LOG_ERR, "子进程fork失败");
        }
    }

    closelog();
    return EXIT_SUCCESS;
}