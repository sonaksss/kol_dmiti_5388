#include "MUL_NN_N.h"
#include <stdlib.h>

/*
  Автор: Александров Ярослав
  Группа: 5388
*/

/*
  Примечание по структуре NUMBN:
    - A[0]   — младшая цифра числа (единицы)
    - A[n-1] — старшая цифра числа
    - n      — количество цифр в числе
*/

/* ─── Заглушки зависимых модулей ─── */

/*
NUMBN* MUL_ND_N(NUMBN* a, NUMBN* d);
*/

/*
NUMBN* MUL_Nk_N(NUMBN* a, int k);
*/

/*
NUMBN* ADD_NN_N(NUMBN* a, NUMBN* b);
*/

/* ─── Вспомогательные функции ─── */

static NUMBN* allocNatural(int size) {
    if (size <= 0) return NULL;
    NUMBN* r = (NUMBN*)malloc(sizeof(NUMBN));
    if (r == NULL) return NULL;
    r->n = size;
    r->A = (int*)calloc(size, sizeof(int));
    if (r->A == NULL) { free(r); return NULL; }
    return r;
}

static void removeLeadingZeros(NUMBN* num) {
    if (num == NULL || num->A == NULL) return;
    while (num->n > 1 && num->A[num->n - 1] == 0) {
        num->n--;
    }
}

/* ─── Основная функция ─── */

/*
  MUL_NN_N

  Умножение столбиком: для каждой цифры b[i] формируем одноэлементное
  число и вызываем MUL_ND_N(a, digit), затем сдвигаем через MUL_Nk_N
  и прибавляем через ADD_NN_N.

  Параметры:
    1) NUMBN* a - первый множитель
    2) NUMBN* b - второй множитель

  Возвращает указатель на новое NUMBN = a * b, или NULL при ошибке
*/
NUMBN* MUL_NN_N(NUMBN* a, NUMBN* b) {
    if (a == NULL || b == NULL) return NULL;
    if (a->n <= 0 || b->n <= 0) return NULL;
    if (a->A == NULL || b->A == NULL) return NULL;

    NUMBN* result = allocNatural(1);
    if (result == NULL) return NULL;

    for (int i = 0; i < b->n; i++) {
        if (b->A[i] == 0) continue;

        /* формируем одноэлементное число из цифры b->A[i] */
        NUMBN* digit = allocNatural(1);
        if (digit == NULL) { free(result->A); free(result); return NULL; }
        digit->A[0] = b->A[i];

        NUMBN* partial = MUL_ND_N(a, digit);
        free(digit->A); free(digit);
        if (partial == NULL) { free(result->A); free(result); return NULL; }

        NUMBN* shifted = MUL_Nk_N(partial, i);
        free(partial->A); free(partial);
        if (shifted == NULL) { free(result->A); free(result); return NULL; }

        NUMBN* sum = ADD_NN_N(result, shifted);
        free(result->A); free(result);
        free(shifted->A); free(shifted);
        if (sum == NULL) return NULL;

        result = sum;
    }

    removeLeadingZeros(result);
    return result;
}
