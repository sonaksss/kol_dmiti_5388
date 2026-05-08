/*
  Автор: Семенов Максим
  Группа: 5388
*/

#include "ABS_Z_Z.h"
#include <stdlib.h>

NUMBZ* ABS_Z_Z(const NUMBZ* num) {
    if (!num) return NULL;

    NUMBZ* result = (NUMBZ*)malloc(sizeof(NUMBZ));
    if (!result) return NULL;

    result->b = 0;                   // знак +
    result->n = num->n;
    result->A = (int*)malloc(result->n * sizeof(int));
    if (!result->A) {
        free(result);
        return NULL;
    }
    for (int i = 0; i < num->n; i++) {
        result->A[i] = num->A[i];
    }
    return result;
}