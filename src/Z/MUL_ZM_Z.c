#include "MUL_ZM_Z.h"
#include <stdlib.h>

/*
  Автор: Александров Ярослав
  Группа: 5388
*/

/*
  Примечание по структуре NUMBZ:
    - b      — знак числа: 0 — плюс (положительное или ноль), 1 — минус
    - n      — количество цифр в числе
    - A[0]   — старшая цифра числа
    - A[n-1] — младшая цифра числа
*/

/*
  isZero

  Проверяет, равно ли целое число нулю.

  Параметры:
    1) NUMBZ* a - проверяемое число

  Возвращает int: 1 - если ноль, 0 - иначе
*/
static int isZero(NUMBZ* a) {
    return a->n == 1 && a->A[0] == 0;
}

/*
  copyInteger

  Создаёт копию целого числа с теми же цифрами и знаком.

  Параметры:
    1) NUMBZ* src - исходное целое число

  Возвращает указатель на новое NUMBZ — копию src
*/
static NUMBZ* copyInteger(NUMBZ* src) {
    NUMBZ* copy = (NUMBZ*)malloc(sizeof(NUMBZ));
    copy->b = src->b;
    copy->n = src->n;
    copy->A = (int*)malloc(src->n * sizeof(int));
    for (int i = 0; i < src->n; i++) {
        copy->A[i] = src->A[i];
    }
    return copy;
}

/*
  MUL_ZM_Z

  Умножение целого числа на (-1): инвертирует знак b.
  Если число равно нулю — возвращает копию без изменения знака,
  так как ноль не имеет знака (b = 0).

  Параметры:
    1) NUMBZ* a - целое число

  Возвращает указатель на новое NUMBZ = a * (-1)
*/
NUMBZ* MUL_ZM_Z(NUMBZ* a) {
    NUMBZ* result = copyInteger(a);

    if (!isZero(a)) {
        result->b = (a->b == 0) ? 1 : 0;
    }

    return result;
}
