/*
 * Практическая работа №7. Задание 1.
 * Обработчик "Ctrl+C": при получении SIGINT процесс
 * приостанавливается и передаёт управление оболочке.
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


/* Флаг: был ли получен сигнал SIGINT */
volatile sig_atomic_t got_sigint = 0;


/*
 * Обработчик сигнала SIGINT (Ctrl+C).
 * Приостанавливает текущий процесс, передавая управление оболочке.
 */
void on_sigint(int signum)
{
    if (signum == SIGINT)
    {
        got_sigint = 1;
        /* Восстанавливаем реакцию на SIGINT (по умолчанию сбрасывается) */
        signal(SIGINT, on_sigint);


        /* Приостанавливаем процесс — управление переходит оболочке */
        printf("\n[Обработчик Ctrl+C] Процесс приостановлен. "
               "Передача управления оболочке...\n");
        fflush(stdout);


        /* SIGSTOP не может быть перехвачен, поэтому используем raise */
        raise(SIGSTOP);


        /* Сюда управление вернётся после SIGCONT */
        printf("[Обработчик Ctrl+C] Процесс возобновлён.\n");
        fflush(stdout);
    }
}


/* Обработчик SIGALRM — имитация кванта времени */
void on_alarm(int signum)
{
    if (signum == SIGALRM)
    {
        printf("\n[Таймер] Квант времени истёк.\n");
        fflush(stdout);
    }
}


int main(void)
{
    /* Устанавливаем обработчики */
    if (signal(SIGINT, on_sigint) == SIG_ERR)
    {
        perror("signal(SIGINT)");
        return 1;
    }


    if (signal(SIGALRM, on_alarm) == SIG_ERR)
    {
        perror("signal(SIGALRM)");
        return 1;
    }


    printf("Программа запущена. PID = %d\n", getpid());
    printf("Нажмите Ctrl+C для приостановки процесса.\n");
    printf("Для продолжения используйте: kill -SIGCONT %d\n\n", getpid());


    /* Основной цикл — имитация работы */
    while (1)
    {
        printf("Процесс работает... (PID = %d)\n", getpid());
        fflush(stdout);


        /* Имитация кванта времени — 2 секунды */
        alarm(2);
        sleep(2);
    }


    return 0;
}
