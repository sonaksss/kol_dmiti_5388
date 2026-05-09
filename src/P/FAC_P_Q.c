#include "FAC_P_Q.h"
#include <stdlib.h>

/*
  Автор: Александров Ярослав
  Группа: 5388
*/

/*
  Примечание по структуре NUMBP:
    - m    — степень многочлена
    - C    — массив коэффициентов NUMBQ (C[i] — коэффициент при x^i)
    - C[0] — свободный член, C[m] — старший коэффициент

  Примечание по структуре NUMBQ:
    - a — числитель (NUMBZ по значению): знак в a.b
    - b — знаменатель (NUMBN по значению), всегда > 0

  Примечание по структуре NUMBN/NUMBZ:
    - A[0]   — младшая цифра числа (единицы)
    - A[n-1] — старшая цифра числа
    - n      — количество цифр в числе
*/

/* ─── Заглушки зависимых модулей ─── */

/*
NUMBN* ABS_Z_N(NUMBZ* a);
*/

/*
NUMBN* GCF_NN_N(NUMBN* a, NUMBN* b);
*/

/*
NUMBN* LCM_NN_N(NUMBN* a, NUMBN* b);
*/

/* ─── Вспомогательные функции ─── */

static int isZeroN(NUMBN* a) {
    if (a == NULL || a->A == NULL || a->n <= 0) return 1;
    return a->n == 1 && a->A[0] == 0;
}

static NUMBN* copyNatural(NUMBN* src) {
    if (src == NULL || src->A == NULL || src->n <= 0) return NULL;
    NUMBN* copy = (NUMBN*)malloc(sizeof(NUMBN));
    if (copy == NULL) return NULL;
    copy->n = src->n;
    copy->A = (int*)malloc(src->n * sizeof(int));
    if (copy->A == NULL) { free(copy); return NULL; }
    for (int i = 0; i < src->n; i++) copy->A[i] = src->A[i];
    return copy;
}

/* ─── Основная функция ─── */

/*
  FAC_P_Q

  Алгоритм:
    1) НОД числителей всех коэффициентов (|C[i].a|) через ABS_Z_N и GCF_NN_N.
    2) НОК знаменателей всех коэффициентов (C[i].b) через LCM_NN_N.
    3) Множитель = НОД / НОК.
    4) Знак = знак старшего ненулевого коэффициента.

  Параметры:
    1) NUMBP* p - многочлен с рациональными коэффициентами

  Возвращает указатель на новое NUMBQ = рациональный множитель, или NULL при ошибке
*/
NUMBQ* FAC_P_Q(NUMBP* p) {
    if (p == NULL) return NULL;
    if (p->C == NULL || p->m < 0) return NULL;

    /* шаг 1: НОД числителей */
    NUMBN* gcdNum = ABS_Z_N(&p->C[0].a);
    if (gcdNum == NULL) return NULL;

    for (int i = 1; i <= p->m; i++) {
        NUMBN* absCoef = ABS_Z_N(&p->C[i].a);
        if (absCoef == NULL) { free(gcdNum->A); free(gcdNum); return NULL; }
        if (isZeroN(absCoef)) {
            free(absCoef->A); free(absCoef);
            continue;
        }
        NUMBN* newGcd = GCF_NN_N(gcdNum, absCoef);
        free(gcdNum->A); free(gcdNum);
        free(absCoef->A); free(absCoef);
        if (newGcd == NULL) return NULL;
        gcdNum = newGcd;
    }

    /* шаг 2: НОК знаменателей */
    NUMBN* lcmDen = copyNatural(&p->C[0].b);
    if (lcmDen == NULL) { free(gcdNum->A); free(gcdNum); return NULL; }

    for (int i = 1; i <= p->m; i++) {
        NUMBN* newLcm = LCM_NN_N(lcmDen, &p->C[i].b);
        free(lcmDen->A); free(lcmDen);
        if (newLcm == NULL) { free(gcdNum->A); free(gcdNum); return NULL; }
        lcmDen = newLcm;
    }

    /* шаг 3: собираем NUMBQ */
    NUMBQ* result = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (result == NULL) {
        free(gcdNum->A); free(gcdNum);
        free(lcmDen->A); free(lcmDen);
        return NULL;
    }

    result->b = *lcmDen; free(lcmDen);
    result->a.n = gcdNum->n;
    result->a.A = gcdNum->A;
    free(gcdNum);

    /* шаг 4: знак = знак старшего ненулевого коэффициента */
    result->a.b = 0;
    for (int i = p->m; i >= 0; i--) {
        if (!(p->C[i].a.n == 1 && p->C[i].a.A[0] == 0)) {
            result->a.b = p->C[i].a.b;
            break;
        }
    }

    return result;
}
