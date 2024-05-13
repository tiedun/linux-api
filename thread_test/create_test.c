#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_LEN 1024

char *buf;

/**
 * @brief 线程input_thread读取标准输入的数据，放入缓存区
 * 
 * @param argv 不需要输入参数，所以固定为NULL
 * @return void* 也不需要返回值
 */
void *input_thread(void *argv)
{
    int i = 0;
    while (1)
    {
        // 从标准输入读取一个字符
        char c = fgetc(stdin);
        // 如果字符不为0也不是换行，写入缓存区
        if (c && c!= '\n')
        {
            buf[i++] = c;
            // 缓存区索引溢出，i归零
            if (i >= BUF_LEN)
            {
                i = 0;
            }
        }
    }
}

/**
 * @brief 线程output_thread从缓存区读取数据写到标准输出，每个字符换行
 * 
 * @param argv 不需要输入参数，所以固定为NULL
 * @return void* 也不需要返回值
 */
void *output_thread(void *argv)
{
    int i = 0;
    while (1)
    {
        if (buf[i])
        {
            // 从缓存区读取一个字节写入标准输出并换行
            fputc(buf[i], stdout);
            fputc('\n', stdout);

            // 清理读取完毕的字节
            buf[i++] = 0;
            if (i >= BUF_LEN)
            {
                i = 0;
            }
        } else {
            sleep(1);
        }
    }
}

int main()
{
    pthread_t pid_input;
    pthread_t pid_output;
    // 分配缓存
    buf = malloc(BUF_LEN);

    // 初始化缓存区
    for (int i = 0; i < BUF_LEN; i++)
    {
        buf[i] = 0;
    }

    // 创建读取线程
    pthread_create(&pid_input, NULL, input_thread, NULL);

    // 创建写出线程
    pthread_create(&pid_output, NULL, output_thread, NULL);

    // 等待指定线程结束，详细作用下一节讲
    pthread_join(pid_input, NULL);
    pthread_join(pid_output, NULL);

    free(buf);

    return 0;
}
