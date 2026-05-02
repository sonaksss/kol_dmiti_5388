#ifndef _ADD_NN_N_H_
#define _ADD_NN_N_H_

#include "numbN.h"

/*
  ADD_NN_N (N-4)

  Сложение двух натуральных чисел.

  Параметры:
    1) NUMBN a - первое натуральное число
    2) NUMBN b - второе натуральное число

  Возвращает NUMBN - сумму a + b.

  ОШИБКА: если a.A == NULL или b.A == NULL, или сбой выделения памяти,
  возвращается структура { n = -1, A = NULL }.
  Вызывающий код ОБЯЗАН проверить результат:
    if (result.n < 0 || result.A == NULL) { // ошибка }
*/
NUMBN ADD_NN_N(NUMBN a, NUMBN b);

#endif