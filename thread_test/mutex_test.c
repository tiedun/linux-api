#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_COUNT 20000

static pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief 对传入值累加1
 *
 * @param argv 传入指针
 * @return void* 无返回值
 */
void *add_thread(void *argv)
{
    int *p = argv;
    // 累加之前加锁，此时其他获取该锁的线程都会被阻塞
    pthread_mutex_lock(&counter_mutex);
    (*p)++;
    // 累加之后释放锁
    pthread_mutex_unlock(&counter_mutex);
    return (void *)0;
}

int main()
{
    pthread_t pid[THREAD_COUNT];

    int num = 0;

    // 用20000个线程对num作累加
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_create(pid + i, NULL, add_thread, &num);
    }

    // 等带所有线程结束
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(pid[i], NULL);
    }

    // 打印累加结果
    printf("累加结果：%d\n", num);

    return 0;
}
