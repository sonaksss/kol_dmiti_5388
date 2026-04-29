#ifndef _N_H_
#define _N_H_

#include "numbN.h"

/*
  Умножение натурального числа на цифру (N-6)

  Параметры:
    1) NUMBN a  - натуральное число (множитель)
    2) int d    - цифра от 0 до 9 (множитель)

  Функция возвращает результат умножения - NUMBN
*/
NUMBN mulNdN(NUMBN a, int d);

#endif