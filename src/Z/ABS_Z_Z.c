/*
  Автор: Семенов Максим
  Группа: 5388
*/

#include "ABS_Z_Z.h"
#include <stdlib.h>

NUMBZ ABS_Z_Z(NUMBZ num) {
    NUMBZ result;
    result.b = 0;          
    result.n = num.n;
    result.A = (int*)malloc(result.n * sizeof(int));
    for (int i = 0; i < num.n; i++) {
        result.A[i] = num.A[i];
    }
    return result;
}