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
NUMBN* ABS_Z_N(NUMBZ* a);
*/

/*
NUMBN* GCF_NN_N(NUMBN* a, NUMBN* b);
*/

/*
NUMBN* DIV_NN_N(NUMBN* a, NUMBN* b);
*/

/*
NUMBZ* DIV_ZZ_Z(NUMBZ* a, NUMBZ* b);
*/

/* ─── Основная функция ─── */

/*
  RED_Q_Q

  Алгоритм:
    1) Если числитель ноль — возвращаем 0/1.
    2) |числитель| через ABS_Z_N.
    3) НОД(|числитель|, знаменатель) через GCF_NN_N.
    4) Числитель / НОД через DIV_ZZ_Z (gcd приводим к NUMBZ).
    5) Знаменатель / НОД через DIV_NN_N.

  Параметры:
    1) NUMBQ* a - рациональное число

  Возвращает указатель на новое NUMBQ — сокращённая дробь, или NULL при ошибке
*/
NUMBQ* RED_Q_Q(NUMBQ* a) {
    if (a == NULL) return NULL;
    if (a->a.A == NULL || a->b.A == NULL) return NULL;
    if (a->a.n <= 0 || a->b.n <= 0) return NULL;

    NUMBQ* result = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (result == NULL) return NULL;

    /* числитель ноль — результат 0/1 */
    if (a->a.n == 1 && a->a.A[0] == 0) {
        result->a.b = 0;
        result->a.n = 1;
        result->a.A = (int*)calloc(1, sizeof(int));
        if (result->a.A == NULL) { free(result); return NULL; }
        result->b.n = 1;
        result->b.A = (int*)calloc(1, sizeof(int));
        if (result->b.A == NULL) { free(result->a.A); free(result); return NULL; }
        result->b.A[0] = 1;
        return result;
    }

    NUMBN* absNum = ABS_Z_N(&a->a);
    if (absNum == NULL) { free(result); return NULL; }

    NUMBN* gcd = GCF_NN_N(absNum, &a->b);
    free(absNum->A); free(absNum);
    if (gcd == NULL) { free(result); return NULL; }

    /* приводим gcd к NUMBZ для вызова DIV_ZZ_Z */
    NUMBZ gcdAsZ;
    gcdAsZ.b = 0;
    gcdAsZ.n = gcd->n;
    gcdAsZ.A = gcd->A;

    NUMBZ* newNum = DIV_ZZ_Z(&a->a, &gcdAsZ);
    NUMBN* newDen = DIV_NN_N(&a->b, gcd);
    free(gcd->A); free(gcd);

    if (newNum == NULL || newDen == NULL) {
        if (newNum) { free(newNum->A); free(newNum); }
        if (newDen) { free(newDen->A); free(newDen); }
        free(result);
        return NULL;
    }

    result->a = *newNum; free(newNum);
    result->b = *newDen; free(newDen);

    return result;
}
