/*
 * Программа 1. Динамическое размещение одномерного массива
 * из N целых чисел. Анализ адресов.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int N = 0;
    int i = 0;

    printf("Введите количество элементов N: ");
    scanf("%d", &N);

    /* Выделение памяти под массив из N целых чисел */
    int *arr = (int*)malloc(N * sizeof(int));
    if (arr == NULL)
    {
        printf("Ошибка выделения памяти!\n");
        return 1;
    }

    /* Заполнение массива */
    for (i = 0; i < N; i++)
    {
        arr[i] = (i + 1) * 10;
    }

    /* Вывод значений и адресов элементов */
    printf("\nБазовый адрес массива (arr): %p\n", (void*)arr);
    printf("Размер одного элемента (sizeof(int)): %zu байт\n\n",
           sizeof(int));

    for (i = 0; i < N; i++)
    {
        printf("arr[%d] = %3d   адрес: %p   смещение: %ld байт\n",
               i, arr[i], (void*)&arr[i],
               (long)((char*)&arr[i] - (char*)arr));
    }

    printf("\nОбщий размер выделенной памяти: %zu байт\n",
           N * sizeof(int));

    /* Освобождение памяти */
    free(arr);
    arr = NULL;   /* обнуляем указатель, чтобы избежать висячего указателя */

    return 0;
}
