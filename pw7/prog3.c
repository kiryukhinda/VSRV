/*
 * Практическая работа №7. Задание 3.
 * Ожидание ввода с клавиатуры в течение 10 секунд.
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


volatile sig_atomic_t timeout = 0;


/* Обработчик SIGALRM */
void alarm_handler(int signum)
{
    if (signum == SIGALRM)
        timeout = 1;
}


int main(void)
{
    char buf[256];
    char *result;


    /* Устанавливаем обработчик SIGALRM */
    if (signal(SIGALRM, alarm_handler) == SIG_ERR)
    {
        perror("signal");
        return 1;
    }


    /* Ставим будильник на 10 секунд */
    alarm(10);


    printf("Введите что-нибудь (у вас 10 секунд): ");
    fflush(stdout);


    errno = 0;
    result = fgets(buf, sizeof(buf), stdin);


    /* Отменяем будильник */
    alarm(0);


    if (timeout)
    {
        printf("\nНет ввода\n");
        printf("[Отладка] fgets вернул NULL, errno = %d (%s)\n",
               errno, strerror(errno));
    }
    else if (result != NULL)
    {
        printf("Спасибо\n");
        printf("Вы ввели: %s", buf);
    }
    else
    {
        printf("\nНет ввода (fgets вернул NULL)\n");
        printf("[Отладка] errno = %d (%s)\n", errno, strerror(errno));
    }


    return 0;
}
