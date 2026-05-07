#include "DIV_ZZ_Z.h"
#include "../N/numbN.h"
#include "ABS_Z_N.h"
#include "POZ_Z_D.h"
#include "../N/DIV_NN_N.h"
#include "../N/ADD_1N_N.h"
#include <stdlib.h>
#include <string.h>

/*
  Автор: Ефанов Денис
  Группа: 5388
*/

/*
  checkRemainder

  Вспомогательная функция: определяет, есть ли ненулевой остаток при делении
  натурального a на натуральное b, зная уже найденное частное q.

  Метод: вычисляет q * b стандартным алгоритмом длинного умножения и сравнивает
  с a. Если q * b == a — остаток нулевой.

  Параметры:
    1) NUMBN* a — делимое
    2) NUMBN* b — делитель
    3) NUMBN* q — частное (= DIV_NN_N(a, b))

  Возвращает: 1 — есть остаток, 0 — остатка нет (или ошибка выделения памяти).
*/
static int checkRemainder(NUMBN* a, NUMBN* b, NUMBN* q)
{
    if (a == NULL || b == NULL || q == NULL)
        return 0;

    int lenQ = q->n + 1;
    int lenB = b->n + 1;
    int lenA = a->n + 1;
    int productLen = lenQ + lenB;

    int* product = (int*)calloc(productLen, sizeof(int));
    if (product == NULL)
        return 0;

    /* Длинное умножение q * b */
    for (int i = 0; i < lenQ; i++)
    {
        int carry = 0;
        for (int j = 0; j < lenB; j++)
        {
            int cur = product[i + j] + q->A[i] * b->A[j] + carry;
            product[i + j] = cur % 10;
            carry = cur / 10;
        }
        /* Распространяем перенос */
        int pos = i + lenB;
        while (carry && pos < productLen)
        {
            int cur = product[pos] + carry;
            product[pos] = cur % 10;
            carry = cur / 10;
            pos++;
        }
    }

    /* Реальная длина произведения (без ведущих нулей) */
    int realLen = productLen;
    while (realLen > 1 && product[realLen - 1] == 0)
        realLen--;

    /* Сравниваем с a */
    int hasRemainder = 0;
    if (realLen != lenA)
    {
        hasRemainder = 1;
    }
    else
    {
        for (int i = 0; i < lenA; i++)
        {
            if (product[i] != a->A[i])
            {
                hasRemainder = 1;
                break;
            }
        }
    }

    free(product);
    return hasRemainder;
}

/*
  DIV_ZZ_Z

  Частное от деления целого на целое (делитель отличен от нуля).

  Реализует математическое деление с округлением в сторону минус бесконечности
  (floor division):
    - Знаки a и b совпадают  => частное неотрицательное.
    - Знаки различны, деление нацело => частное отрицательное.
    - Знаки различны, остаток ненулевой => частное отрицательное,
      |частное| = ⌊|a|/|b|⌋ + 1  (т.е. ADD_1N_N применяется к модулю).

  Параметры:
    1) NUMBZ* a — делимое (целое число)
    2) NUMBZ* b — делитель (целое число, отличное от нуля)

  Возвращает: NUMBZ* — частное a / b, или NULL при ошибке входных данных.
*/
NUMBZ* DIV_ZZ_Z(NUMBZ* a, NUMBZ* b)
{
    if (a == NULL || b == NULL)
        return NULL;

    /* Получаем модули |a| и |b| */
    NUMBN* absA = ABS_Z_N(a);
    NUMBN* absB = ABS_Z_N(b);
    if (absA == NULL || absB == NULL)
    {
        free(absA);
        free(absB);
        return NULL;
    }

    /* Натуральное частное q = ⌊|a| / |b|⌋ */
    NUMBN* quotient = DIV_NN_N(absA, absB);
    if (quotient == NULL)
    {
        free(absA->A); free(absA);
        free(absB->A); free(absB);
        return NULL;
    }

    /*
      Знаки операндов.
      POZ_Z_D возвращает: 2 — положительное, 0 — ноль, 1 — отрицательное.
    */
    int signA = POZ_Z_D(a);
    int signB = POZ_Z_D(b);
    int differentSigns = (signA == 1 && signB == 2) ||
                         (signA == 2 && signB == 1);

    /*
      Корректировка: при разных знаках и ненулевом остатке
      увеличиваем модуль частного на 1.
    */
    if (differentSigns && checkRemainder(absA, absB, quotient))
    {
        NUMBN* corrected = ADD_1N_N(quotient);
        free(quotient->A);
        free(quotient);
        if (corrected == NULL)
        {
            free(absA->A); free(absA);
            free(absB->A); free(absB);
            return NULL;
        }
        quotient = corrected;
    }

    /* Формируем результат NUMBZ */
    NUMBZ* result = (NUMBZ*)malloc(sizeof(NUMBZ));
    if (result == NULL)
    {
        free(quotient->A); free(quotient);
        free(absA->A); free(absA);
        free(absB->A); free(absB);
        return NULL;
    }

    result->n = quotient->n;
    result->A = (int*)malloc((quotient->n + 1) * sizeof(int));
    if (result->A == NULL)
    {
        free(result);
        free(quotient->A); free(quotient);
        free(absA->A); free(absA);
        free(absB->A); free(absB);
        return NULL;
    }

    memcpy(result->A, quotient->A, (quotient->n + 1) * sizeof(int));

    /* Знак: 0 — неотрицательное, 1 — отрицательное */
    int isZero = (quotient->n == 0 && quotient->A[0] == 0);
    if (isZero)
        result->b = 0;
    else if (differentSigns)
        result->b = 1;
    else
        result->b = 0;

    free(quotient->A); free(quotient);
    free(absA->A); free(absA);
    free(absB->A); free(absB);

    return result;
}
