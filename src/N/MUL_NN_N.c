#include "MUL_NN_N.h"
#include <stdlib.h>

/*
  Автор: Александров Ярослав
  Группа: 5388
*/

/*
  Примечание по структуре NUMBN:
    - A[0]   — старшая цифра числа
    - A[n-1] — младшая цифра числа
    - n      — количество цифр в числе
*/

/* ─── Заглушки зависимых модулей ─── */

/*
static NUMBN* MUL_ND_N_stub(NUMBN* a, int d) {
    return ...;
}
*/

/*
static NUMBN* MUL_Nk_N_stub(NUMBN* a, int k) {
    return ...;
}
*/

/*
static NUMBN* ADD_NN_N_stub(NUMBN* a, NUMBN* b) {
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

static NUMBN* MUL_ND_N(NUMBN* a, int d) {
    NUMBN* result = allocNatural(a->n + 1);

    int carry = 0;
    for (int i = 0; i < a->n; i++) {
        int prod = a->A[a->n - 1 - i] * d + carry;
        result->A[result->n - 1 - i] = prod % 10;
        carry = prod / 10;
    }
    result->A[0] = carry;

    removeLeadingZeros(result);
    return result;
}

static NUMBN* MUL_Nk_N(NUMBN* a, int k) {
    NUMBN* result = allocNatural(a->n + k);
    for (int i = 0; i < a->n; i++) {
        result->A[i] = a->A[i];
    }
    return result;
}

static NUMBN* ADD_NN_N(NUMBN* a, NUMBN* b) {
    int maxN = (a->n > b->n) ? a->n : b->n;
    NUMBN* result = allocNatural(maxN + 1);

    int carry = 0;
    for (int i = 0; i < maxN || carry; i++) {
        int aDigit = (i < a->n) ? a->A[a->n - 1 - i] : 0;
        int bDigit = (i < b->n) ? b->A[b->n - 1 - i] : 0;
        int sum = aDigit + bDigit + carry;
        result->A[result->n - 1 - i] = sum % 10;
        carry = sum / 10;
    }

    removeLeadingZeros(result);
    return result;
}

/* ─── Основная функция ─── */

/*
  MUL_NN_N

  Умножение длинных чисел методом «умножение столбиком»:
  для каждой цифры b[i] (с конца массива — от младшей к старшей)
  вычисляется частичное произведение a * b[i] через MUL_ND_N,
  сдвигается на i позиций через MUL_Nk_N,
  и прибавляется к накопленному результату через ADD_NN_N.

  Параметры:
    1) NUMBN* a - первый множитель
    2) NUMBN* b - второй множитель

  Возвращает указатель на новое NUMBN = a * b
*/
NUMBN* MUL_NN_N(NUMBN* a, NUMBN* b) {
    NUMBN* result = allocNatural(1);

    for (int i = 0; i < b->n; i++) {
        int digit = b->A[b->n - 1 - i];
        if (digit == 0) continue;

        NUMBN* partial = MUL_ND_N(a, digit);
        NUMBN* shifted = MUL_Nk_N(partial, i);
        NUMBN* sum     = ADD_NN_N(result, shifted);

        free(result->A); free(result);
        free(partial->A); free(partial);
        free(shifted->A); free(shifted);
        result = sum;
    }

    removeLeadingZeros(result);
    return result;
}
