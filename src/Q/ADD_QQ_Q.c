#include "ADD_QQ_Q.h"
#include "RED_Q_Q.h"
#include <stdlib.h>

/*
  Автор: Александров Ярослав
  Группа: 5388
*/

/*
  Примечание по структуре NUMBQ:
    - a — числитель (NUMBZ по значению): знак в a.b, цифры в a.A[0..n-1]
    - b — знаменатель (NUMBN по значению): всегда > 0

  Примечание по структуре NUMBN/NUMBZ:
    - A[0]   — младшая цифра числа (единицы)
    - A[n-1] — старшая цифра числа
    - n      — количество цифр в числе
*/

/* ─── Заглушки зависимых модулей ─── */

/*
NUMBN* MUL_NN_N(NUMBN* a, NUMBN* b);
*/

/*
NUMBZ* MUL_ZN_Z(NUMBZ* a, NUMBN* b);
*/

/*
NUMBZ* ADD_ZZ_Z(NUMBZ* a, NUMBZ* b);
*/

/* ─── Основная функция ─── */

/*
  ADD_QQ_Q

  Алгоритм:
    1) числитель = a->a * b->b + b->a * a->b  (MUL_ZN_Z, ADD_ZZ_Z)
    2) знаменатель = a->b * b->b              (MUL_NN_N)
    3) сокращаем через RED_Q_Q

  Параметры:
    1) NUMBQ* a - первое слагаемое
    2) NUMBQ* b - второе слагаемое

  Возвращает указатель на новое NUMBQ = a + b (сокращённая форма), или NULL при ошибке
*/
NUMBQ* ADD_QQ_Q(NUMBQ* a, NUMBQ* b) {
    if (a == NULL || b == NULL) return NULL;
    if (a->a.A == NULL || a->b.A == NULL) return NULL;
    if (b->a.A == NULL || b->b.A == NULL) return NULL;
    if (a->a.n <= 0 || a->b.n <= 0) return NULL;
    if (b->a.n <= 0 || b->b.n <= 0) return NULL;

    /* числитель: a->a * b->b + b->a * a->b */
    NUMBZ* anum_bden = MUL_ZN_Z(&a->a, &b->b);
    if (anum_bden == NULL) return NULL;

    NUMBZ* bnum_aden = MUL_ZN_Z(&b->a, &a->b);
    if (bnum_aden == NULL) { free(anum_bden->A); free(anum_bden); return NULL; }

    NUMBZ* newNum = ADD_ZZ_Z(anum_bden, bnum_aden);
    free(anum_bden->A); free(anum_bden);
    free(bnum_aden->A); free(bnum_aden);
    if (newNum == NULL) return NULL;

    /* знаменатель: a->b * b->b */
    NUMBN* newDen = MUL_NN_N(&a->b, &b->b);
    if (newDen == NULL) { free(newNum->A); free(newNum); return NULL; }

    NUMBQ raw;
    raw.a = *newNum; free(newNum);
    raw.b = *newDen; free(newDen);

    NUMBQ* result = RED_Q_Q(&raw);
    free(raw.a.A);
    free(raw.b.A);

    return result;
}
