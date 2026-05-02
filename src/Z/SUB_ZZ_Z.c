/*
  Автор: Семенов Максим
  Группа: 5388
*/

#include "SUB_ZZ_Z.h"
#include "ADD_ZZ_Z.h"
#include "MUL_ZM_Z.h"
#include <stdlib.h>

NUMBZ SUB_ZZ_Z(NUMBZ A, NUMBZ B) {
    NUMBZ minusB = MUL_ZM_Z(B);      
    NUMBZ result = ADD_ZZ_Z(A, minusB);

    free(minusB.A);
    return result;
}