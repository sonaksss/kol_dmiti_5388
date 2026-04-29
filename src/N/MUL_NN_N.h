#ifndef MUL_NN_N_H
#define MUL_NN_N_H

#include "numbN.h"

/*
  Автор: Александров Ярослав
  Группа: 5388
*/

/*
  MUL_NN_N

  Умножение двух натуральных чисел.

  Параметры:
    1) NUMBN* a - первый множитель
    2) NUMBN* b - второй множитель

  Возвращает указатель на новое NUMBN = a * b
*/
NUMBN* MUL_NN_N(NUMBN* a, NUMBN* b);

#endif /* MUL_NN_N_H */
