#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *task(void *arg)
{
    printf("Thread started\n");

    // 默认取消类型为延迟，无需设置

    // 模拟工作
    printf("Working...\n");
    sleep(1);             // 模拟工作
    pthread_testcancel(); // 取消点函数
    printf("After Cancelled\n");

    return NULL;
}

int main()
{
    pthread_t tid;
    void *res;

    // 创建线程
    pthread_create(&tid, NULL, task, NULL);

    // 取消子线程
    if (pthread_cancel(tid) != 0){
        perror("pthread_cancel");
    }

    // 等待子线程终止并获取其退出状态
    pthread_join(tid, &res);

    // 检查子线程是否被取消
    if (res == PTHREAD_CANCELED)
    {
        printf("Thread was canceled\n");
    }
    else
    {
        printf("Thread was not canceled, exit code: %ld\n", (long)res);
    }

    return 0;
}
