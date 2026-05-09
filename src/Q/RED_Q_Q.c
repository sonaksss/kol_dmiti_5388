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
static NUMBN* DIV_NN_N(NUMBN* a, NUMBN* b) {
    return ...;
}
*/

/*
static NUMBZ* DIV_ZZ_Z(NUMBZ* a, NUMBZ* b) {
    return ...;
}
*/

/* ─── Основная функция ─── */

/*
  RED_Q_Q

  Алгоритм:
    1) Берём |числитель| через ABS_Z_N.
    2) Вычисляем НОД(|числитель|, знаменатель) через GCF_NN_N.
    3) Делим числитель на НОД через DIV_ZZ_Z.
    4) Делим знаменатель на НОД через DIV_NN_N.

  Параметры:
    1) NUMBQ* a - рациональное число

  Возвращает указатель на новое NUMBQ — сокращённая дробь
*/
NUMBQ* RED_Q_Q(NUMBQ* a) {
    NUMBQ* result = (NUMBQ*)malloc(sizeof(NUMBQ));

    /* если числитель ноль — возвращаем 0/1 */
    if (a->a.n == 1 && a->a.A[0] == 0) {
        result->a.b = 0;
        result->a.n = 1;
        result->a.A = (int*)calloc(1, sizeof(int));
        result->b.n = 1;
        result->b.A = (int*)calloc(1, sizeof(int));
        result->b.A[0] = 1;
        return result;
    }

    NUMBN* absNum = ABS_Z_N(&a->a);
    NUMBN* gcd    = GCF_NN_N(absNum, &a->b);
    free(absNum->A); free(absNum);

    /* числитель / НОД — используем DIV_ZZ_Z, передавая gcd как целое */
    NUMBZ gcdAsZ;
    gcdAsZ.b = 0;
    gcdAsZ.n = gcd->n;
    gcdAsZ.A = gcd->A;

    NUMBZ* newNum = DIV_ZZ_Z(&a->a, &gcdAsZ);
    NUMBN* newDen = DIV_NN_N(&a->b, gcd);
    free(gcd->A); free(gcd);

    result->a = *newNum;
    free(newNum);
    result->b = *newDen;
    free(newDen);

    return result;
}
