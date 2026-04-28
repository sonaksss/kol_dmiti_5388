#ifndef LCM_NN_N_H
#define LCM_NN_N_H

#include "numbN.h"

/*
  Автор: Александров Ярослав
  Группа: 5388
*/

/*
  leastCommonMultiple

  Наименьшее общее кратное двух натуральных чисел.

  Параметры:
    1) NUMBN* a - первое натуральное число
    2) NUMBN* b - второе натуральное число

  Возвращает указатель на новое NUMBN = НОК(a, b)
*/
NUMBN* leastCommonMultiple(NUMBN* a, NUMBN* b);

#endif /* LCM_NN_N_H */
