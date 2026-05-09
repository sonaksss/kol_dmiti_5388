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
    - a — числитель (NUMBZ по значению): знак в поле a.b
    - b — знаменатель (NUMBN по значению), всегда > 0

  Алгоритм:
    Для многочлена с коэффициентами a_i/b_i:
      множитель = НОД(|a_0|, |a_1|, ..., |a_m|) / НОК(b_0, b_1, ..., b_m)
    Знак множителя = знак старшего ненулевого коэффициента.
*/

/* ─── Заглушки зависимых модулей ─── */

/*
static NUMBN* ABS_Z_N(NUMBZ* a) {
    return ...;
}
*/

/*
static NUMBN* GCF_NN_N(NUMBN* a, NUMBN* b) {
    return ...;
}
*/

/*
static NUMBN* LCM_NN_N(NUMBN* a, NUMBN* b) {
    return ...;
}
*/

/* ─── Вспомогательные функции ─── */

static int isZeroN(NUMBN* a) {
    return a->n == 1 && a->A[0] == 0;
}

static NUMBN* copyNatural(NUMBN* src) {
    NUMBN* copy = (NUMBN*)malloc(sizeof(NUMBN));
    copy->n = src->n;
    copy->A = (int*)malloc(src->n * sizeof(int));
    for (int i = 0; i < src->n; i++) copy->A[i] = src->A[i];
    return copy;
}

/* ─── Основная функция ─── */

/*
  FAC_P_Q

  Алгоритм:
    1) НОД числителей всех коэффициентов (по абсолютной величине) через GCF_NN_N и ABS_Z_N.
    2) НОК знаменателей всех коэффициентов через LCM_NN_N.
    3) Собираем результат: числитель = НОД, знаменатель = НОК.
    4) Знак = знак старшего ненулевого коэффициента.

  Параметры:
    1) NUMBP* p - многочлен с рациональными коэффициентами

  Возвращает указатель на новое NUMBQ = рациональный множитель
*/
NUMBQ* FAC_P_Q(NUMBP* p) {
    /* шаг 1: НОД числителей */
    NUMBN* gcdNum = ABS_Z_N(&p->C[0].a);

    for (int i = 1; i <= p->m; i++) {
        NUMBN* absCoef = ABS_Z_N(&p->C[i].a);
        if (isZeroN(absCoef)) {
            free(absCoef->A); free(absCoef);
            continue;
        }
        NUMBN* newGcd = GCF_NN_N(gcdNum, absCoef);
        free(gcdNum->A); free(gcdNum);
        free(absCoef->A); free(absCoef);
        gcdNum = newGcd;
    }

    /* шаг 2: НОК знаменателей */
    NUMBN* lcmDen = copyNatural(&p->C[0].b);

    for (int i = 1; i <= p->m; i++) {
        NUMBN* newLcm = LCM_NN_N(lcmDen, &p->C[i].b);
        free(lcmDen->A); free(lcmDen);
        lcmDen = newLcm;
    }

    /* шаг 3: собираем NUMBQ */
    NUMBQ* result = (NUMBQ*)malloc(sizeof(NUMBQ));
    result->b = *lcmDen;
    free(lcmDen);

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
