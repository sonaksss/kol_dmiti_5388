#include <stdlib.h>
#include <string.h>
#include "ADD_1N_N.h"

/*
  Автор: Нигай Борис
  Группа: 5388
*/

/*
  ADD_1N_N

  Добавление 1 к натуральному числу.

  Число хранится в массиве A[]
  A[0] - младший разряд
  A[n-1] - старший.
  Программа реализует ручное прибавление единицы с переносом:
  1) прибавляем 1 к A[0], если нет переноса - готово
  2) если есть перенос, продолжаем по разрядам
  3) если перенос вышел за пределы числа то увеличиваем разрядность

  Параметры:
    1) NUMBN num - исходное натуральное число
    2) carry - начальный перенос(прибавляемая единица)

  Возвращает NUMBN - новое натуральное число, равное num + 1
*/

NUMBN ADD_1N_N(NUMBN num) {
    if (num.A == NULL) {
        return num;
    }

    NUMBN result;
    result.n = num.n;
    result.A = (int*)malloc(result.n * sizeof(int));
    if (result.A == NULL) {
        return num;
    }
    memcpy(result.A, num.A, result.n * sizeof(int));

    int carry = 1;
    for (int i = 0; i < result.n && carry != 0; i++) {
        int sum = result.A[i] + carry;
        result.A[i] = sum % 10;
        carry = sum / 10;
    }

    if (carry) {
        int* tmp = (int*)realloc(result.A, (result.n + 1) * sizeof(int));
        if (tmp == NULL) {
            free(result.A);
            return num;
        }
        result.A = tmp;
        result.A[result.n] = carry;
        result.n += 1;
    }

    return result;
}
