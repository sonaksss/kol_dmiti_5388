#ifndef COM_NN_D_H
#define COM_NN_D_H

#include "numbN.h"

/*
  Автор: Александров Ярослав
  Группа: 5388
*/

/*
  compareNaturalNumbers

  Сравнение двух натуральных чисел.

  Параметры:
    1) NUMBN* a - первое натуральное число
    2) NUMBN* b - второе натуральное число

  Возвращает int: 2 - если a > b,
                  0 - если a == b,
                  1 - если a < b
*/
int compareNaturalNumbers(NUMBN* a, NUMBN* b);

#endif /* COM_NN_D_H */
