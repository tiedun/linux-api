#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

int main()
{
    char *mq_name = "/father_son_mq";

    struct mq_attr attr;

    attr.mq_flags = 0;     // 被忽略，任意值都可以
    attr.mq_maxmsg = 10;   // 设置消息队列可以容纳的最大消息数
    attr.mq_msgsize = 100; // 设置容许的单条消息最大长度
    attr.mq_curmsgs = 0;   // 被忽略，任意值都可以

    // 创建消息队列
    mqd_t mqdes = mq_open(mq_name, O_RDWR | O_CREAT, 0666, &attr);

    if (mqdes == (mqd_t)-1)
    {
        perror("mq_open");
        return 1;
    }

    pid_t pid = fork();

    if (pid > 0)
    {
        char sendBuf[100];
        struct timespec time_info;

        for (int i = 0; i < 10; i++)
        {
            // 清空发送缓冲区
            memset(sendBuf, 0, 100);

            // 获取当前时间并获取5s后的timespec对象
            clock_gettime(CLOCK_REALTIME, &time_info);
            time_info.tv_sec += 5;

            // 发送数据，数据长度不可以超过mq_msgsize
            sprintf(sendBuf, "父进程的第 %d 次发送\n", (i + 1));
            if (mq_timedsend(mqdes, sendBuf, strlen(sendBuf), 0, &time_info) == -1)
            {
                perror("mq_timedsend");
            }

            printf("父进程发送完毕，休眠1s\n");

            sleep(1);
        }
    }
    else if (pid == 0)
    {
        char readBuf[100];
        struct timespec time_info;

        for (int i = 0; i < 10; i++)
        {
            // 清空接收缓冲区
            memset(readBuf, 0, 100);

            // 获取当前时间并获取15s后的timespec对象
            clock_gettime(CLOCK_REALTIME, &time_info);
            time_info.tv_sec += 15;

            // 接收数据并打印到控制台，期望接收的数据长度不可以小于mq_msgsize
            if (mq_timedreceive(mqdes, readBuf, 100, NULL, &time_info) == -1)
            {
                perror("mq_timedreceive");
            }

            printf("子进程接收到数据: %s\n", readBuf);
        }
    }
    else
    {
        perror("fork");
    }

    // 无论是父进程还是子进程都应该关闭队列描述符，只有引用全部释放，消息队列才会被真正销毁
    close(mqdes);

    // 消息队列的清除只应执行一次
    if (pid > 0)
    {
        mq_unlink(mq_name);
    }

    return 0;
}
