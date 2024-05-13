#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_COUNT 20000

/**
 * @brief 对传入值累加1
 *
 * @param argv 传入指针
 * @return void* 无返回值
 */
void *add_thread(void *argv)
{
    int *p = argv;
    (*p)++;
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

    // 等待所有线程结束
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(pid[i], NULL);
    }

    // 打印累加结果
    printf("累加结果：%d\n", num);

    return 0;
}
