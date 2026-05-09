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
    - A[0]   — младшая цифра числа (единицы)
    - A[n-1] — старшая цифра числа
*/

static NUMBZ* copyInteger(NUMBZ* src) {
    if (src == NULL) return NULL;
    NUMBZ* copy = (NUMBZ*)malloc(sizeof(NUMBZ));
    if (copy == NULL) return NULL;
    copy->b = src->b;
    copy->n = src->n;
    copy->A = (int*)malloc(src->n * sizeof(int));
    if (copy->A == NULL) { free(copy); return NULL; }
    for (int i = 0; i < src->n; i++) {
        copy->A[i] = src->A[i];
    }
    return copy;
}

/*
  MUL_ZM_Z

  Инвертирует поле b (знак). Ноль не меняет знак (b остаётся 0).

  Параметры:
    1) NUMBZ* a - целое число

  Возвращает указатель на новое NUMBZ = a * (-1), или NULL при ошибке
*/
NUMBZ* MUL_ZM_Z(NUMBZ* a) {
    if (a == NULL) return NULL;
    if (a->n <= 0 || a->A == NULL) return NULL;

    NUMBZ* result = copyInteger(a);
    if (result == NULL) return NULL;

    /* ноль знак не меняет */
    if (!(a->n == 1 && a->A[0] == 0)) {
        result->b = (a->b == 0) ? 1 : 0;
    }

    return result;
}
