#include "SUB_NN_N.h"
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

/*
  SUB_NN_N

  Вычитание столбиком с заимствованием (borrow) от младшего разряда к старшему.
  A[0] — младшая цифра, A[n-1] — старшая, идём от начала массива.
  Предполагается, что a >= b.

  Параметры:
    1) NUMBN* a - уменьшаемое
    2) NUMBN* b - вычитаемое (b <= a)

  Возвращает указатель на новое NUMBN = a - b, или NULL при ошибке
*/
NUMBN* SUB_NN_N(NUMBN* a, NUMBN* b) {
    if (a == NULL || b == NULL) return NULL;
    if (a->n <= 0 || b->n <= 0) return NULL;
    if (a->A == NULL || b->A == NULL) return NULL;

    NUMBN* result = allocNatural(a->n);
    if (result == NULL) return NULL;

    int borrow = 0;
    for (int i = 0; i < a->n; i++) {
        int aDigit = a->A[i];
        int bDigit = (i < b->n) ? b->A[i] : 0;
        int diff = aDigit - bDigit - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result->A[i] = diff;
    }

    removeLeadingZeros(result);
    return result;
}
