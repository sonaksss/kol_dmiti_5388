#include <stdlib.h>
#include "LCM_NN_N.h"

/*
  Автор: Нигай Борис
  Группа: 5388
*/

/*
  LCM_NN_N (N-14)

  Вычисление наименьшего общего кратного (НОК) двух натуральных чисел.

  Формула: НОК(a, b) = (a / НОД(a, b)) * b
  Деление выполняется ДО умножения, чтобы избежать лишнего роста числа.

  Зависит от:
    - GCF_NN_N (N-13) - НОД натуральных чисел
    - DIV_NN_N (N-11) - целочисленное деление натуральных чисел
    - MUL_NN_N (N-8)  - умножение натуральных чисел

  Параметры:
    1) NUMBN a - первое натуральное число
    2) NUMBN b - второе натуральное число

  Возвращает NUMBN - НОК(a, b).

  ОШИБКА: если a.A == NULL или b.A == NULL,
  возвращается структура { n = -1, A = NULL }.
  Вызывающий код ОБЯЗАН проверить результат:
    if (result.n < 0 || result.A == NULL) { // ошибка }
*/

/* 
  Заглушки: N-13, N-11, N-8
NUMBN GCF_NN_N(NUMBN a, NUMBN b) { return a; }
NUMBN DIV_NN_N(NUMBN a, NUMBN b) { return a; }
NUMBN MUL_NN_N(NUMBN a, NUMBN b) { return a; }
*/

static NUMBN makeError() {
    NUMBN err;
    err.n = -1;
    err.A = NULL;
    return err;
}

NUMBN LCM_NN_N(NUMBN a, NUMBN b) {
    if (a.A == NULL || b.A == NULL) {
        return makeError();
    }

    NUMBN gcd = GCF_NN_N(a, b);
    if (gcd.A == NULL) {
        return makeError();
    }

    NUMBN a_div = DIV_NN_N(a, gcd);
    free(gcd.A);
    if (a_div.A == NULL) {
        return makeError();
    }

    NUMBN result = MUL_NN_N(a_div, b);
    free(a_div.A);
    if (result.A == NULL) {
        return makeError();
    }

    return result;
}