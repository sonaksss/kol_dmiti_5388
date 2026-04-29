#ifndef SUB_NN_N_H
#define SUB_NN_N_H

#include "numbN.h"

/*
  Автор: Александров Ярослав
  Группа: 5388
*/

/*
  SUB_NN_N

  Вычитание из первого большего натурального числа второго меньшего или равного.

  Параметры:
    1) NUMBN* a - уменьшаемое (должно быть >= b)
    2) NUMBN* b - вычитаемое

  Возвращает указатель на новое NUMBN = a - b
*/
NUMBN* SUB_NN_N(NUMBN* a, NUMBN* b);

#endif /* SUB_NN_N_H */
