#include "MUL_ZZ_Z.h"
#include "MUL_ZM_Z.h"
#include "numbN.h"
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

/* ─── Заглушки зависимых модулей ─── */

/*
int POZ_Z_D(NUMBZ* a);
*/

/*
NUMBN* ABS_Z_N(NUMBZ* a);
*/

/*
NUMBN* MUL_NN_N(NUMBN* a, NUMBN* b);
*/

/* ─── Основная функция ─── */

/*
  MUL_ZZ_Z

  Алгоритм:
    1) Определяем знаки через POZ_Z_D.
    2) Если один из множителей ноль — возвращаем ноль.
    3) Берём абсолютные величины через ABS_Z_N.
    4) Перемножаем через MUL_NN_N.
    5) Знак: одинаковые знаки → плюс (b=0), разные → минус (b=1).

  Параметры:
    1) NUMBZ* a - первый множитель
    2) NUMBZ* b - второй множитель

  Возвращает указатель на новое NUMBZ = a * b, или NULL при ошибке
*/
NUMBZ* MUL_ZZ_Z(NUMBZ* a, NUMBZ* b) {
    if (a == NULL || b == NULL) return NULL;
    if (a->n <= 0 || b->n <= 0) return NULL;
    if (a->A == NULL || b->A == NULL) return NULL;

    NUMBZ* result = (NUMBZ*)malloc(sizeof(NUMBZ));
    if (result == NULL) return NULL;

    int signA = POZ_Z_D(a);
    int signB = POZ_Z_D(b);

    /* один из множителей ноль — результат ноль */
    if (signA == 0 || signB == 0) {
        result->b = 0;
        result->n = 1;
        result->A = (int*)calloc(1, sizeof(int));
        if (result->A == NULL) { free(result); return NULL; }
        return result;
    }

    NUMBN* absA = ABS_Z_N(a);
    if (absA == NULL) { free(result); return NULL; }

    NUMBN* absB = ABS_Z_N(b);
    if (absB == NULL) { free(absA->A); free(absA); free(result); return NULL; }

    NUMBN* prod = MUL_NN_N(absA, absB);
    free(absA->A); free(absA);
    free(absB->A); free(absB);
    if (prod == NULL) { free(result); return NULL; }

    result->n = prod->n;
    result->A = prod->A;
    free(prod);

    result->b = (signA == signB) ? 0 : 1;
    return result;
}
