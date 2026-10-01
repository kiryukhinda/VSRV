/*
 * Программа 2. Динамическое размещение двумерного массива
 * размером N x M целых чисел. Анализ адресов.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int N = 0, M = 0;
    int i = 0, j = 0;

    printf("Введите количество строк N: ");
    scanf("%d", &N);
    printf("Введите количество столбцов M: ");
    scanf("%d", &M);

    /* 1. Выделение памяти под массив указателей на строки */
    int **matrix = (int**)malloc(N * sizeof(int*));
    if (matrix == NULL)
    {
        printf("Ошибка выделения памяти (указатели на строки)!\n");
        return 1;
    }

    /* 2. Выделение памяти под каждую строку */
    for (i = 0; i < N; i++)
    {
        matrix[i] = (int*)malloc(M * sizeof(int));
        if (matrix[i] == NULL)
        {
            printf("Ошибка выделения памяти (строка %d)!\n", i);
            /* Освобождаем уже выделенное */
            for (j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            return 1;
        }
    }

    /* 3. Заполнение массива */
    for (i = 0; i < N; i++)
        for (j = 0; j < M; j++)
            matrix[i][j] = (i + 1) * 10 + (j + 1);

    /* 4. Вывод значений и адресов */
    printf("\n=== Анализ адресов двумерного массива ===\n");
    printf("Адрес массива указателей (matrix): %p\n", (void*)matrix);
    printf("Размер указателя (sizeof(int*)): %zu байт\n",
           sizeof(int*));
    printf("Размер int: %zu байт\n\n", sizeof(int));

    printf("--- Адреса строк (указателей matrix[i]) ---\n");
    for (i = 0; i < N; i++)
    {
        printf("matrix[%d] = %p   (адрес самой ячейки: %p)\n",
               i, (void*)matrix[i], (void*)&matrix[i]);
    }

    printf("\n--- Адреса элементов ---\n");
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
            printf("matrix[%d][%d] = %3d   адрес: %p\n",
                   i, j, matrix[i][j], (void*)&matrix[i][j]);
        }
        printf("\n");
    }

    /* 5. Освобождение памяти */
    for (i = 0; i < N; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
    matrix = NULL;

    return 0;
}
