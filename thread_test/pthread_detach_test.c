#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *task(void *arg)
{
    printf("Thread started\n");
    sleep(2); // 模拟线程工作
    printf("Thread finished\n");
    return NULL;
}

int main()
{
    pthread_t tid;

    // 创建线程
    pthread_create(&tid, NULL, task, NULL);

    // 使用 pthread_detach 让线程自动回收资源
    pthread_detach(tid);

    // 主线程继续工作
    printf("Main thread continues\n");
    sleep(3); // 需要注意的是，pthread_detach不会等待子线程结束，如果在后者执行完毕之前主线程退出，则整个进程退出，子线程被强制终止，因此需要等待足够的时间确保子线程完成自己的任务
    printf("Main thread ending\n");

    return 0;
}
