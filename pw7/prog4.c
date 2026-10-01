/*
 * Практическая работа №7. Задание 4.
 * Реализация функции sleep(n) через alarm() и pause().
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>


/* Флаг: был ли получен SIGALRM */
static volatile sig_atomic_t alarm_fired = 0;


/*
 * Обработчик SIGALRM — устанавливает флаг завершения ожидания.
 */
void alarm_handler(int signum)
{
    (void)signum;
    alarm_fired = 1;
}


/*
 * Собственная функция sleep(n).
 * Задерживает выполнение программы на n секунд.
 * При получении другого сигнала (не SIGALRM) —
 * перезапускает ожидание с сохранением оставшегося времени.
 *
 * Возвращает: 0 — если выдержана полная задержка,
 *             остаток секунд — если sleep был прерван.
 */
unsigned int my_sleep(unsigned int n)
{
    unsigned int remaining = n;


    /* Сохраняем старый обработчик */
    void (*old_handler)(int) = signal(SIGALRM, alarm_handler);
    if (old_handler == SIG_ERR)
    {
        perror("signal(SIGALRM)");
        return n;
    }


    /* Устанавливаем будильник */
    alarm(remaining);


    /* Ожидаем сигнала */
    while (remaining > 0 && !alarm_fired)
    {
        pause();   /* засыпаем до получения любого сигнала */


        if (alarm_fired)
        {
            /* SIGALRM получен — ожидание завершено */
            break;
        }


        /*
         * Получен другой сигнал — перезапускаем ожидание.
         * alarm() возвращает оставшееся время.
         */
        remaining = alarm(0);   /* отменяем старый будильник */
        if (remaining == 0)
        {
            /* Время уже истекло */
            break;
        }
        alarm(remaining);       /* переустанавливаем будильник */
    }


    /* Восстанавливаем старый обработчик */
    signal(SIGALRM, old_handler);


    return alarm_fired ? 0 : remaining;
}


/* Обработчик SIGUSR1 — для демонстрации рестарта */
void usr1_handler(int signum)
{
    (void)signum;
    printf("\n[my_sleep] Получен SIGUSR1 — рестарт ожидания.\n");
    fflush(stdout);
}


int main(void)
{
    /* Устанавливаем обработчик SIGUSR1 */
    if (signal(SIGUSR1, usr1_handler) == SIG_ERR)
    {
        perror("signal(SIGUSR1)");
        return 1;
    }


    printf("=== Демонстрация my_sleep ===\n\n");


    /* Тест 1: обычная задержка */
    printf("Тест 1: my_sleep(3) — ожидание 3 секунды...\n");
    fflush(stdout);
    unsigned int r1 = my_sleep(3);
    printf("my_sleep вернул: %u\n\n", r1);


    /* Тест 2: задержка с прерыванием */
    printf("Тест 2: my_sleep(5). Через 2 секунды пошлите SIGUSR1.\n");
    printf("Команда: kill -SIGUSR1 %d\n", getpid());
    fflush(stdout);
    unsigned int r2 = my_sleep(5);
    printf("my_sleep вернул: %u\n\n", r2);


    printf("=== Завершение ===\n");
    return 0;
}
