/*
 * Программа 3. Вариант 7.
 * Функция принимает два арифметических вектора и возвращает
 * вектор с наибольшей длиной и его длину.
 * Вызов функции осуществляется через указатель на функцию.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * Вычисление длины (нормы) вектора.
 */
double vector_length(const double* arr, int n)
{
    double sum = 0.0;
    int i = 0;
    for (i = 0; i < n; i++)
        sum += arr[i] * arr[i];
    return sqrt(sum);
}

/*
 * Функция сравнения двух векторов.
 * Возвращает через указатели:
 *   result — адрес вектора с наибольшей длиной,
 *   len    — длину этого вектора,
 *   size   — его размерность.
 * Через return возвращает номер вектора (1 или 2).
 */
int get_longest_vector(const double* v1, int n1,
                       const double* v2, int n2,
                       const double** result,
                       double* len,
                       int* size)
{
    double len1 = vector_length(v1, n1);
    double len2 = vector_length(v2, n2);

    printf("\nДлина первого вектора:  %lf\n", len1);
    printf("Длина второго вектора:  %lf\n", len2);

    if (len1 >= len2)
    {
        *result = v1;
        *len    = len1;
        *size   = n1;
        return 1;
    }
    else
    {
        *result = v2;
        *len    = len2;
        *size   = n2;
        return 2;
    }
}

/* Вывод вектора */
void print_vector(const char* name, const double* v, int n)
{
    int i = 0;
    printf("%s = { ", name);
    for (i = 0; i < n; i++)
    {
        printf("%.2lf", v[i]);
        if (i < n - 1) printf(", ");
    }
    printf(" }\n");
}

int main(void)
{
    int n1 = 5, n2 = 4;
    int i = 0;

    /* Динамическое выделение памяти под векторы */
    double* v1 = (double*)malloc(sizeof(double) * n1);
    double* v2 = (double*)malloc(sizeof(double) * n2);

    if (v1 == NULL || v2 == NULL)
    {
        printf("Ошибка выделения памяти!\n");
        free(v1);
        free(v2);
        return 1;
    }

    /* Заполнение векторов */
    for (i = 0; i < n1; i++) v1[i] = i + 1;
    for (i = 0; i < n2; i++) v2[i] = (i + 1) * 2.0;

    printf("Исходные векторы:\n");
    print_vector("v1", v1, n1);
    print_vector("v2", v2, n2);

    /* Объявление указателя на функцию */
    int (*fptr)(const double*, int,
                const double*, int,
                const double**, double*, int*) = NULL;

    /* Присваиваем адрес функции */
    fptr = get_longest_vector;

    /* Переменные для результатов */
    const double* longest = NULL;
    double length = 0.0;
    int    size   = 0;
    int    which  = 0;

    /* Вызов функции через указатель */
    which = fptr(v1, n1, v2, n2, &longest, &length, &size);

    /* Вывод результатов */
    printf("\n=== Результат ===\n");
    printf("Вектор с наибольшей длиной: v%d\n", which);
    printf("Его длина:                   %lf\n", length);
    printf("Его размерность:             %d\n", size);
    print_vector("Сам вектор", longest, size);

    /* Освобождение памяти */
    free(v1);
    free(v2);

    return 0;
}
