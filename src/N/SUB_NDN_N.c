#include "SUB_NDN_N.h"
#include "COM_NN_D.h"
#include "MUL_ND_N.h"
#include "SUB_NN_N.h"
#include <stdlib.h>

/*
  Автор: Ефанов Денис
  Группа: 5388

  SUB_NDN_N

  Вычитание из натурального другого натурального, умноженного на цифру,
  для случая с неотрицательным результатом: result = a - b * d.

  Предусловие: a >= b * d.
  Цифра d должна быть в диапазоне 0..9.

  Параметры:
    1) NUMBN* a — уменьшаемое
    2) NUMBN* b — натуральное число, которое умножается на d
    3) int d    — цифра-множитель (0..9)

  Возвращает NUMBN* — результат a - b*d.
  В случае ошибки возвращает NULL.
*/
NUMBN* SUB_NDN_N(NUMBN* a, NUMBN* b, int d) {
    if (a == NULL || a->A == NULL) return NULL;
    if (b == NULL || b->A == NULL) return NULL;
    if (d < 0 || d > 9)           return NULL;

    /* d == 0: a - 0 = a, возвращаем копию a */
    if (d == 0) {
        NUMBN* result = (NUMBN*)malloc(sizeof(NUMBN));
        if (!result) return NULL;
        result->n = a->n;
        result->A = (int*)malloc(a->n * sizeof(int));
        if (!result->A) { free(result); return NULL; }
        for (int i = 0; i < a->n; i++) result->A[i] = a->A[i];
        return result;
    }

    /* Вычислить b * d через обновлённый MUL_ND_N, возвращающий указатель */
    NUMBN* bd = MUL_ND_N(b, d);
    if (bd == NULL) return NULL;

    /* Убедиться, что a >= b*d, иначе нарушено предусловие */
    if (COM_NN_D(a, bd) == 1) {
        free(bd->A);
        free(bd);
        return NULL;
    }

    /* Вычислить a - b*d */
    NUMBN* result = SUB_NN_N(a, bd);
    free(bd->A);
    free(bd);

    return result;
}