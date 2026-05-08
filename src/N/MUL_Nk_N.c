/*
  Автор: Семенов Максим
  Группа: 5388
*/

#include "MUL_Nk_N.h"
#include <stdlib.h>

NUMBN* MUL_Nk_N(const NUMBN* num, int k) {
    if (k < 0 || num == NULL) return NULL;

    NUMBN* result = (NUMBN*)malloc(sizeof(NUMBN));
    if (result == NULL) return NULL;

    result->n = num->n + k;
    result->A = (int*)malloc(result->n * sizeof(int));
    if (result->A == NULL) {
        free(result);
        return NULL;
    }

    // Младшие k разрядов — нули
    for (int i = 0; i < k; i++) {
        result->A[i] = 0;
    }
    // Копируем исходные цифры со сдвигом
    for (int i = 0; i < num->n; i++) {
        result->A[i + k] = num->A[i];
    }
    return result;
}