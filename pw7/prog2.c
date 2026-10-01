/*
 * Практическая работа №7. Задание 2.
 * Программа выводит файл /etc/termcap.
 * Перехватывает SIGINT и запрашивает действие пользователя.
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


#define BUF_SIZE 512


static int fd = -1;          /* файловый дескриптор */
static volatile sig_atomic_t need_query = 0;


/*
 * Обработчик сигнала SIGINT.
 * Устанавливает флаг необходимости запроса.
 * Реакция на сигнал восстанавливается.
 */
void onintr(int sig)
{
    (void)sig;
    signal(SIGINT, onintr);   /* восстановить реакцию */
    need_query = 1;
}


int main(void)
{
    char buf[BUF_SIZE];
    ssize_t n;
    char answer;


    /* Открываем файл /etc/termcap */
    fd = open("/etc/termcap", O_RDONLY);
    if (fd < 0)
    {
        perror("open /etc/termcap");
        return 1;
    }


    /* Устанавливаем обработчик SIGINT */
    if (signal(SIGINT, onintr) == SIG_ERR)
    {
        perror("signal(SIGINT)");
        close(fd);
        return 1;
    }


    printf("Вывод файла /etc/termcap. Нажмите Ctrl+C для паузы.\n\n");


    /* Цикл вывода файла */
    while ((n = read(fd, buf, sizeof(buf))) > 0)
    {
        /* Если был получен SIGINT — запрашиваем действие */
        if (need_query)
        {
            need_query = 0;
            printf("\n\n=== Продолжать? (y/n/r) ===\n");
            printf("  y — продолжить\n");
            printf("  n — завершить\n");
            printf("  r — начать с начала\n");
            printf("Ваш выбор: ");
            fflush(stdout);


            /* Считываем ответ */
            if (scanf(" %c", &answer) != 1)
            {
                answer = 'n';
            }
            /* Очищаем буфер ввода */
            while (getchar() != '\n' && !feof(stdin));


            switch (answer)
            {
                case 'y':
                case 'Y':
                    printf("\nПродолжаем вывод...\n\n");
                    break;


                case 'n':
                case 'N':
                    printf("\nЗавершение программы.\n");
                    close(fd);
                    return 0;


                case 'r':
                case 'R':
                    printf("\nНачинаем вывод с начала файла...\n\n");
                    if (lseek(fd, 0L, SEEK_SET) == (off_t)-1)
                    {
                        perror("lseek");
                        close(fd);
                        return 1;
                    }
                    break;


                default:
                    printf("\nНеизвестная команда. Продолжаем вывод.\n\n");
                    break;
            }
        }


        /* Выводим прочитанный блок */
        if (write(STDOUT_FILENO, buf, n) != n)
        {
            perror("write");
            close(fd);
            return 1;
        }
    }


    if (n < 0)
    {
        perror("read");
    }


    printf("\n\n=== Конец файла ===\n");
    close(fd);
    return 0;
}
