#include "SUB_NN_N.h"
#include "COM_NN_D.h"
#include <stdlib.h>

/*
  Автор: Александров Ярослав
  Группа: 5388
*/

/*
  allocNatural

  Выделяет память под новое натуральное число с заданным количеством цифр.
  Все цифры инициализируются нулём.

  Примечание по структуре NUMBN:
    - A[0]   — старшая цифра числа
    - A[n-1] — младшая цифра числа
    - n      — количество цифр в числе

  Параметры:
    1) int size - количество цифр

  Возвращает указатель на новое NUMBN
*/
static NUMBN* allocNatural(int size) {
    NUMBN* result = (NUMBN*)malloc(sizeof(NUMBN));
    result->n = size;
    result->A = (int*)calloc(size, sizeof(int));
    return result;
}

/*
  removeLeadingZeros

  Убирает ведущие нули, уменьшая число цифр и сдвигая массив.
  Минимальная длина — 1 (число «0» хранится как A[0]=0, n=1).

  Параметры:
    1) NUMBN* num - число, в котором нужно убрать ведущие нули

  Возвращает void
*/
static void removeLeadingZeros(NUMBN* num) {
    int leading = 0;
    while (leading < num->n - 1 && num->A[leading] == 0) {
        leading++;
    }
    if (leading > 0) {
        int newN = num->n - leading;
        int* newA = (int*)malloc(newN * sizeof(int));
        for (int i = 0; i < newN; i++) {
            newA[i] = num->A[i + leading];
        }
        free(num->A);
        num->A = newA;
        num->n = newN;
    }
}

/*
  subtractNaturalFromNatural

  Вычитание столбиком с заимствованием (borrow) от младшего разряда к старшему.
  A[0] — старшая цифра, A[n-1] — младшая, поэтому идём с конца массива.
  Предполагается, что a >= b (проверяется вызывающей стороной через compareNaturalNumbers).

  Параметры:
    1) NUMBN* a - уменьшаемое
    2) NUMBN* b - вычитаемое (b <= a)

  Возвращает указатель на новое NUMBN = a - b
*/
NUMBN* subtractNaturalFromNatural(NUMBN* a, NUMBN* b) {
    NUMBN* result = allocNatural(a->n);

    int borrow = 0;
    /* Идём от младшего разряда (конец массива) к старшему (начало) */
    for (int i = 0; i < a->n; i++) {
        int aDigit = a->A[a->n - 1 - i];
        int bDigit = (i < b->n) ? b->A[b->n - 1 - i] : 0;
        int diff = aDigit - bDigit - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result->A[result->n - 1 - i] = diff;
    }

    removeLeadingZeros(result);
    return result;
}
