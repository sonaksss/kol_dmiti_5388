#include "NMR_P_P.h"
#include "GCF_PP_P.h"
#include "DER_P_P.h"
#include "DIV_PP_P.h"
#include <stdlib.h>

/*
  Автор: Ефанов Денис
  Группа: 5388
*/

/*
  isZeroPolynomial

  Вспомогательная функция: проверяет, является ли многочлен нулевым
  (все коэффициенты равны нулю или степень равна 0 и единственный
  коэффициент — ноль).

  Параметры:
    1) NUMBP* p — указатель на проверяемый многочлен

  Возвращает 1 если многочлен нулевой, 0 иначе.
*/
static int isZeroPolynomial(NUMBP* p) {
    if (!p) return 1;
    /* Проверяем все коэффициенты на ноль */
    for (int i = 0; i <= p->m; i++) {
        /* Числитель коэффициента ненулевой => многочлен не нулевой */
        int allZero = 1;
        for (int j = 0; j < p->C[i].a.n; j++) {
            if (p->C[i].a.A[j] != 0) {
                allZero = 0;
                break;
            }
        }
        if (!allZero) return 0;
    }
    return 1;
}

/*
  NMR_P_P

  Преобразование многочлена: кратные корни в простые.
  Алгоритм: result = P / GCF(P, P')
  где P' — производная многочлена P.

  Если P — нулевой многочлен или P' — нулевой (константа),
  то НОД равен самому P, и результат — константный многочлен 1
  (или сам P, если P тоже константа).

  Параметры:
    1) NUMBP* p — указатель на исходный многочлен

  Возвращает указатель на новый NUMBP с простыми корнями,
  или NULL при ошибке.
*/
NUMBP* NMR_P_P(NUMBP* p) {
    if (!p) return NULL;

    /* Вычисляем производную P' */
    NUMBP* derivative = DER_P_P(p);
    if (!derivative) return NULL;

    /* Если производная нулевая (P — константа), то у P нет корней вообще.
       Возвращаем копию P как есть. */
    if (isZeroPolynomial(derivative)) {
        free(derivative->C);
        free(derivative);

        /* Создаём копию исходного многочлена */
        NUMBP* copy = (NUMBP*)malloc(sizeof(NUMBP));
        if (!copy) return NULL;
        copy->m = p->m;
        copy->C = (NUMBQ*)malloc((p->m + 1) * sizeof(NUMBQ));
        if (!copy->C) {
            free(copy);
            return NULL;
        }
        for (int i = 0; i <= p->m; i++) {
            copy->C[i] = p->C[i];
        }
        return copy;
    }

    /* Вычисляем НОД(P, P') */
    NUMBP* gcd = GCF_PP_P(p, derivative);
    free(derivative->C);
    free(derivative);

    if (!gcd) return NULL;

    /* Если НОД нулевой — ошибка */
    if (isZeroPolynomial(gcd)) {
        free(gcd->C);
        free(gcd);
        return NULL;
    }

    /* Если НОД — константа (степень 0), то у P нет кратных корней.
       Возвращаем копию P. */
    if (gcd->m == 0) {
        free(gcd->C);
        free(gcd);

        NUMBP* copy = (NUMBP*)malloc(sizeof(NUMBP));
        if (!copy) return NULL;
        copy->m = p->m;
        copy->C = (NUMBQ*)malloc((p->m + 1) * sizeof(NUMBQ));
        if (!copy->C) {
            free(copy);
            return NULL;
        }
        for (int i = 0; i <= p->m; i++) {
            copy->C[i] = p->C[i];
        }
        return copy;
    }

    /* Делим P на НОД(P, P'): result = P / GCF(P, P') */
    NUMBP* result = DIV_PP_P(p, gcd);
    free(gcd->C);
    free(gcd);

    if (!result) return NULL;

    return result;
}