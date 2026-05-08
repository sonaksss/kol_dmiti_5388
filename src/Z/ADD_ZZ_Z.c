#include <stdlib.h>
#include <string.h>
#include "ADD_ZZ_Z.h"

/*
  Автор: Нигай Борис
  Группа: 5388
*/

/*
  ADD_ZZ_Z (Z-6)

  Сложение двух целых чисел.

  Знак числа хранится в поле b: 0 - плюс, 1 - минус.
  Массив цифр A[]: A[0] - младший разряд, A[n-1] - старший.

  Алгоритм:
    1) Определяем знаки через POZ_Z_D:
         2 - положительное, 0 - ноль, 1 - отрицательное
    2) Если одно из чисел ноль - возвращаем копию другого
    3) Если знаки одинаковы:
         результат = ADD_NN_N(|a|, |b|), знак тот же
    4) Если знаки разные - сравниваем модули через COM_NN_D:
         |a| > |b|: результат = SUB_NN_N(|a|, |b|), знак от a
         |a| < |b|: результат = SUB_NN_N(|b|, |a|), знак от b
         |a| == |b|: результат = 0

  Зависит от:
    - POZ_Z_D  (Z-2) - знак целого числа
    - ABS_Z_N  (Z-1) - модуль целого -> натуральное
    - COM_NN_D (N-1) - сравнение натуральных: 2 больше, 1 меньше, 0 равно
    - ADD_NN_N (N-4) - сложение натуральных
    - SUB_NN_N (N-5) - вычитание натуральных
    - MUL_ZM_Z (Z-3) - умножение целого на (-1)

  Параметры:
    1) NUMBZ* a - указатель на первое целое число
    2) NUMBZ* b - указатель на второе целое число

  Возвращает NUMBZ* - указатель на сумму a + b.

  ОШИБКА: возвращает NULL при невалидных входных данных
  или сбое выделения памяти.
  Вызывающий код ОБЯЗАН проверить результат:
    if (result == NULL) { // ошибка }
  Вызывающий код ОБЯЗАН освободить память после использования:
    free(result->A);
    free(result);
*/

/*
  Заглушки:
int    POZ_Z_D  (NUMBZ* a)             { return 2; }
NUMBN* ABS_Z_N  (NUMBZ* a)             { NUMBN* n = malloc(sizeof(NUMBN)); n->n = a->n; n->A = a->A; return n; }
int    COM_NN_D (NUMBN* a, NUMBN* b)   { return 0; }
NUMBN* ADD_NN_N (NUMBN* a, NUMBN* b)   { return a; }
NUMBN* SUB_NN_N (NUMBN* a, NUMBN* b)   { return a; }
NUMBZ* MUL_ZM_Z (NUMBZ* a)            { return a; }
*/

static NUMBZ* copyNumbZ(NUMBZ* src) {
    NUMBZ* copy = (NUMBZ*)malloc(sizeof(NUMBZ));
    if (copy == NULL) {
        return NULL;
    }
    copy->A = (int*)malloc(src->n * sizeof(int));
    if (copy->A == NULL) {
        free(copy);
        return NULL;
    }
    memcpy(copy->A, src->A, src->n * sizeof(int));
    copy->n = src->n;
    copy->b = src->b;
    return copy;
}

static NUMBZ* makeZero() {
    NUMBZ* zero = (NUMBZ*)malloc(sizeof(NUMBZ));
    if (zero == NULL) {
        return NULL;
    }
    zero->A = (int*)malloc(sizeof(int));
    if (zero->A == NULL) {
        free(zero);
        return NULL;
    }
    zero->A[0] = 0;
    zero->n = 1;
    zero->b = 0;
    return zero;
}

static NUMBZ* wrapNatural(NUMBN* natNum, int sign) {
    NUMBZ* result = (NUMBZ*)malloc(sizeof(NUMBZ));
    if (result == NULL) {
        return NULL;
    }
    result->A = natNum->A;
    result->n = natNum->n;
    result->b = sign;
    free(natNum);
    return result;
}

NUMBZ* ADD_ZZ_Z(NUMBZ* a, NUMBZ* b) {
    if (a == NULL || b == NULL || a->A == NULL || b->A == NULL) {
        return NULL;
    }

    int sign_a = POZ_Z_D(a);  /* 2 — плюс, 0 — ноль, 1 — минус */
    int sign_b = POZ_Z_D(b);

    if (sign_a == 0) return copyNumbZ(b);
    if (sign_b == 0) return copyNumbZ(a);

    NUMBN* abs_a = ABS_Z_N(a);
    NUMBN* abs_b = ABS_Z_N(b);
    if (abs_a == NULL || abs_b == NULL) {
        free(abs_a);
        free(abs_b);
        return NULL;
    }

    NUMBZ* result = NULL;

    if (sign_a == sign_b) {
        NUMBN* sum = ADD_NN_N(abs_a, abs_b);
        free(abs_a->A); free(abs_a);
        free(abs_b->A); free(abs_b);
        if (sum == NULL) {
            return NULL;
        }
        result = wrapNatural(sum, a->b);
    } else {
        int cmp = COM_NN_D(abs_a, abs_b);

        if (cmp == 0) {
            free(abs_a->A); free(abs_a);
            free(abs_b->A); free(abs_b);
            result = makeZero();
        } else if (cmp == 2) {
            NUMBN* diff = SUB_NN_N(abs_a, abs_b);
            free(abs_a->A); free(abs_a);
            free(abs_b->A); free(abs_b);
            if (diff == NULL) {
                return NULL;
            }
            result = wrapNatural(diff, a->b);
        } else {
            NUMBN* diff = SUB_NN_N(abs_b, abs_a);
            free(abs_a->A); free(abs_a);
            free(abs_b->A); free(abs_b);
            if (diff == NULL) {
                return NULL;
            }
            result = wrapNatural(diff, b->b);
        }
    }

    return result;
}