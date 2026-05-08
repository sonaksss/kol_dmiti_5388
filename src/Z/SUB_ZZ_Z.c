/*
  Автор: Семенов Максим
  Группа: 5388
*/

#include "SUB_ZZ_Z.h"
#include "ADD_ZZ_Z.h"
#include "MUL_ZM_Z.h"
#include <stdlib.h>

NUMBZ* SUB_ZZ_Z(const NUMBZ* A, const NUMBZ* B) {
    if (!A || !B) return NULL;

    NUMBZ* minusB = MUL_ZM_Z(B);           // -B
    if (!minusB) return NULL;

    NUMBZ* result = ADD_ZZ_Z(A, minusB);

    // Освобождаем minusB
    free(minusB->A);
    free(minusB);

    return result;
}