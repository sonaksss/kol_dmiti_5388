#include "MUL_ZZ_Z.h"
#include "MUL_ZM_Z.h"
#include "numbN.h"
#include <stdlib.h>

/*
  Автор: Александров Ярослав
  Группа: 5388
*/

/*
  Примечание по структуре NUMBZ:
    - b      — знак числа: 0 — плюс (положительное или ноль), 1 — минус
    - n      — количество цифр в числе
    - A[0]   — старшая цифра числа
    - A[n-1] — младшая цифра числа

  Примечание по структуре NUMBN:
    - n      — количество цифр в числе
    - A[0]   — старшая цифра числа
    - A[n-1] — младшая цифра числа
*/

/* ─── Заглушки зависимых модулей ─── */

/*
static int POZ_Z_D(NUMBZ* a) {
    return ...;
}
*/

/*
static NUMBN* ABS_Z_N(NUMBZ* a) {
    return ...;
}
*/

/*
static NUMBN* MUL_NN_N(NUMBN* a, NUMBN* b) {
    return ...;
}
*/

/* ─── Вспомогательные функции ─── */

static NUMBN* allocNatural(int size) {
    NUMBN* r = (NUMBN*)malloc(sizeof(NUMBN));
    r->n = size;
    r->A = (int*)calloc(size, sizeof(int));
    return r;
}

static void removeLeadingZeros(NUMBN* num) {
    int leading = 0;
    while (leading < num->n - 1 && num->A[leading] == 0) leading++;
    if (leading > 0) {
        int newN = num->n - leading;
        int* newA = (int*)malloc(newN * sizeof(int));
        for (int i = 0; i < newN; i++) newA[i] = num->A[i + leading];
        free(num->A);
        num->A = newA;
        num->n = newN;
    }
}

static int isZeroZ(NUMBZ* a) {
    return a->n == 1 && a->A[0] == 0;
}

/*
  POZ_Z_D

  Локальная реализация модуля POZ_Z_D.
  При интеграции: удалить эту функцию, подключить #include "POZ_Z_D.h".

  Параметры:
    1) NUMBZ* a - целое число

  Возвращает int: 2 - положительное, 0 - ноль, 1 - отрицательное
*/
static int POZ_Z_D(NUMBZ* a) {
    if (isZeroZ(a)) return 0;
    return (a->b == 0) ? 2 : 1;
}

/*
  ABS_Z_N

  Локальная реализация модуля ABS_Z_N.
  Возвращает абсолютную величину целого числа как NUMBN.
  При интеграции: удалить эту функцию, подключить #include "ABS_Z_N.h".

  Параметры:
    1) NUMBZ* a - целое число

  Возвращает указатель на новое NUMBN = |a|
*/
static NUMBN* ABS_Z_N(NUMBZ* a) {
    NUMBN* result = allocNatural(a->n);
    for (int i = 0; i < a->n; i++) result->A[i] = a->A[i];
    return result;
}

/*
  MUL_NN_N

  Локальная реализация модуля MUL_NN_N.
  Умножение двух натуральных чисел столбиком.
  При интеграции: удалить эту функцию, подключить #include "MUL_NN_N.h".

  Параметры:
    1) NUMBN* a - первый множитель
    2) NUMBN* b - второй множитель

  Возвращает указатель на новое NUMBN = a * b
*/
static NUMBN* MUL_NN_N(NUMBN* a, NUMBN* b) {
    NUMBN* result = allocNatural(1);

    for (int i = 0; i < b->n; i++) {
        int d = b->A[b->n - 1 - i];
        if (d == 0) continue;

        /* частичное произведение a * d */
        NUMBN* partial = allocNatural(a->n + 1);
        int carry = 0;
        for (int j = 0; j < a->n; j++) {
            int prod = a->A[a->n - 1 - j] * d + carry;
            partial->A[partial->n - 1 - j] = prod % 10;
            carry = prod / 10;
        }
        partial->A[0] = carry;
        removeLeadingZeros(partial);

        /* сдвиг на i разрядов */
        NUMBN* shifted = allocNatural(partial->n + i);
        for (int j = 0; j < partial->n; j++) shifted->A[j] = partial->A[j];

        /* сложение с накопителем */
        int maxN = (result->n > shifted->n) ? result->n : shifted->n;
        NUMBN* sum = allocNatural(maxN + 1);
        carry = 0;
        for (int j = 0; j < maxN || carry; j++) {
            int x = (j < result->n)  ? result->A[result->n - 1 - j]   : 0;
            int y = (j < shifted->n) ? shifted->A[shifted->n - 1 - j] : 0;
            int s = x + y + carry;
            sum->A[sum->n - 1 - j] = s % 10;
            carry = s / 10;
        }
        removeLeadingZeros(sum);

        free(result->A);  free(result);
        free(partial->A); free(partial);
        free(shifted->A); free(shifted);
        result = sum;
    }

    removeLeadingZeros(result);
    return result;
}

/* ─── Основная функция ─── */

/*
  MUL_ZZ_Z

  Умножение двух целых чисел.
  Алгоритм:
    1) Определяем знаки через POZ_Z_D.
    2) Если один из множителей ноль — возвращаем ноль.
    3) Берём абсолютные величины через ABS_Z_N.
    4) Перемножаем их через MUL_NN_N.
    5) Знак: одинаковые знаки → плюс, разные → минус.

  Параметры:
    1) NUMBZ* a - первый множитель
    2) NUMBZ* b - второй множитель

  Возвращает указатель на новое NUMBZ = a * b
*/
NUMBZ* MUL_ZZ_Z(NUMBZ* a, NUMBZ* b) {
    NUMBZ* result = (NUMBZ*)malloc(sizeof(NUMBZ));

    int signA = POZ_Z_D(a);
    int signB = POZ_Z_D(b);

    if (signA == 0 || signB == 0) {
        result->b = 0;
        result->n = 1;
        result->A = (int*)calloc(1, sizeof(int));
        return result;
    }

    NUMBN* absA = ABS_Z_N(a);
    NUMBN* absB = ABS_Z_N(b);
    NUMBN* prod = MUL_NN_N(absA, absB);
    free(absA->A); free(absA);
    free(absB->A); free(absB);

    result->n = prod->n;
    result->A = prod->A;
    free(prod);

    result->b = (signA == signB) ? 0 : 1;
    return result;
}
