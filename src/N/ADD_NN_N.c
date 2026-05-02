#include <stdlib.h>
#include "ADD_NN_N.h"

/*
  Автор: Нигай Борис
  Группа: 5388
*/

/*
  ADD_NN_N (N-4)

  Сложение двух натуральных чисел.

  Числа хранятся в массивах A[]:
    A[0] - младший разряд, A[n-1] - старший.

  Алгоритм - поразрядное сложение с переносом:
    1) идём по разрядам от младшего к старшему
    2) складываем цифры обоих чисел и перенос из предыдущего разряда
    3) если после старшего разряда остался перенос - добавляем новый разряд

  Зависит от:
    COM_NN_D (N-1) - не используется напрямую в алгоритме сложения,
    но должна быть доступна как часть модуля N.

  Параметры:
    1) NUMBN a - первое натуральное число
    2) NUMBN b - второе натуральное число

  Возвращает NUMBN - сумму a + b.

  ОШИБКА: если a.A == NULL или b.A == NULL, или сбой выделения памяти,
  возвращается структура { n = -1, A = NULL }.
  Вызывающий код ОБЯЗАН проверить результат:
    if (result.n < 0 || result.A == NULL) { // ошибка }
*/

/* Заглушка: N-1
int COM_NN_D(NUMBN a, NUMBN b) { return 0; }
*/

static NUMBN makeError() {
    NUMBN err;
    err.n = -1;
    err.A = NULL;
    return err;
}

NUMBN ADD_NN_N(NUMBN a, NUMBN b) {
    if (a.A == NULL || b.A == NULL) {
        return makeError();
    }

    int maxLen = (a.n > b.n) ? a.n : b.n;

    NUMBN result;
    result.n = maxLen + 1;
    result.A = (int*)malloc(result.n * sizeof(int));
    if (result.A == NULL) {
        return makeError();
    }

    int carry = 0;
    for (int i = 0; i < maxLen; i++) {
        int digit_a = (i < a.n) ? a.A[i] : 0;
        int digit_b = (i < b.n) ? b.A[i] : 0;
        int sum = digit_a + digit_b + carry;
        result.A[i] = sum % 10;
        carry = sum / 10;
    }

    result.A[maxLen] = carry;

    if (result.A[result.n - 1] == 0) {
        result.n--;
    }

    return result;
}