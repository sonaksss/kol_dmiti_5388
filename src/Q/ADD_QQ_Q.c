#include "ADD_QQ_Q.h"
#include "RED_Q_Q.h"
#include <stdlib.h>

/*
  Автор: Александров Ярослав
  Группа: 5388
*/

/*
  Примечание по структуре NUMBQ:
    - a — числитель (NUMBZ по значению): знак в поле a.b, цифры в a.A[0..n-1]
    - b — знаменатель (NUMBN по значению): всегда > 0, цифры в b.A[0..n-1]
*/

/* ─── Заглушки зависимых модулей ─── */

/*
static NUMBN* MUL_NN_N(NUMBN* a, NUMBN* b) {
    return ...;
}
*/

/*
static NUMBZ* MUL_ZN_Z(NUMBZ* a, NUMBN* b) {
    return ...;
}
*/

/*
static NUMBZ* ADD_ZZ_Z(NUMBZ* a, NUMBZ* b) {
    return ...;
}
*/

/* ─── Основная функция ─── */

/*
  ADD_QQ_Q

  Алгоритм:
    1) числитель = a->a * b->b + b->a * a->b  (через MUL_ZN_Z и ADD_ZZ_Z)
    2) знаменатель = a->b * b->b              (через MUL_NN_N)
    3) сокращаем результат через RED_Q_Q

  Параметры:
    1) NUMBQ* a - первое слагаемое
    2) NUMBQ* b - второе слагаемое

  Возвращает указатель на новое NUMBQ = a + b (сокращённая форма)
*/
NUMBQ* ADD_QQ_Q(NUMBQ* a, NUMBQ* b) {
    /* числитель: a->a * b->b + b->a * a->b */
    NUMBZ* anum_bden = MUL_ZN_Z(&a->a, &b->b);
    NUMBZ* bnum_aden = MUL_ZN_Z(&b->a, &a->b);
    NUMBZ* newNum    = ADD_ZZ_Z(anum_bden, bnum_aden);
    free(anum_bden->A); free(anum_bden);
    free(bnum_aden->A); free(bnum_aden);

    /* знаменатель: a->b * b->b */
    NUMBN* newDen = MUL_NN_N(&a->b, &b->b);

    /* собираем дробь и сокращаем */
    NUMBQ raw;
    raw.a = *newNum;
    free(newNum);
    raw.b = *newDen;
    free(newDen);

    NUMBQ* result = RED_Q_Q(&raw);
    free(raw.a.A);
    free(raw.b.A);

    return result;
}
