/*
  Автор: Семенов Максим
  Группа: 5388
*/

#include "MUL_Nk_N.h"
#include <stdlib.h>

NUMBN MUL_Nk_N(const NUMBN* num, int k) {
    NUMBN result;
    result.n = num->n + k;
    result.A = (int*)malloc(result.n * sizeof(int));
    if (result.A == NULL) {
        result.n = 0;
        return result;
    }

    for (int i = 0; i < k; i++) {
        result.A[i] = 0;
    }

    for (int i = 0; i < num->n; i++) {
        result.A[i + k] = num->A[i];
    }

    return result;
}
