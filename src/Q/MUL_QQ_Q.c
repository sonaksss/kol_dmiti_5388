#include <stdlib.h>
#include "MUL_QQ_Q.h"

/*
  Автор: Нигай Борис
  Группа: 5388
*/

/*
  MUL_QQ_Q (Q-7)

  Умножение двух рациональных чисел.

  Рациональное число NUMBQ хранится как пара:
    a - числитель (NUMBZ, целое)
    b - знаменатель (NUMBN, натуральное, всегда > 0)

  Формула:
    (p/q) * (r/s) = (p * r) / (q * s)

  Алгоритм:
    1) Перемножаем числители: p * r через MUL_ZZ_Z
    2) Перемножаем знаменатели: q * s через MUL_NN_N
    3) Сокращаем результат через RED_Q_Q

  Зависит от:
    - MUL_ZZ_Z (Z-8) - умножение целых чисел
    - MUL_NN_N (N-8) - умножение натуральных (для знаменателей)
    - RED_Q_Q  (Q-1) - сокращение дроби

  Параметры:
    1) NUMBQ* a - указатель на первую дробь
    2) NUMBQ* b - указатель на вторую дробь

  Возвращает NUMBQ* - указатель на произведение a * b в сокращённом виде.

  ОШИБКА: возвращает NULL при невалидных входных данных
  или сбое выделения памяти.
  Вызывающий код ОБЯЗАН проверить результат:
    if (result == NULL) { // ошибка }
  Вызывающий код ОБЯЗАН освободить память после использования:
    free(result->a.A);
    free(result->b.A);
    free(result);
*/

/*
  Заглушки:
NUMBZ* MUL_ZZ_Z(NUMBZ* a, NUMBZ* b) { return a; }
NUMBN* MUL_NN_N(NUMBN* a, NUMBN* b) { return a; }
NUMBQ* RED_Q_Q(NUMBQ* q)            { return q; }
*/

NUMBQ* MUL_QQ_Q(NUMBQ* a, NUMBQ* b) {
    if (a == NULL || b == NULL) {
        return NULL;
    }
    if (a->a.A == NULL || a->b.A == NULL ||
        b->a.A == NULL || b->b.A == NULL) {
        return NULL;
    }

    NUMBZ* num = MUL_ZZ_Z(&a->a, &b->a);
    if (num == NULL) {
        return NULL;
    }

    NUMBN* den = MUL_NN_N(&a->b, &b->b);
    if (den == NULL) {
        free(num->A);
        free(num);
        return NULL;
    }

    NUMBQ* raw = (NUMBQ*)malloc(sizeof(NUMBQ));
    if (raw == NULL) {
        free(num->A); free(num);
        free(den->A); free(den);
        return NULL;
    }

    raw->a = *num;
    raw->b = *den;
    free(num);      
    free(den);

    NUMBQ* result = RED_Q_Q(raw);
    free(raw->a.A);
    free(raw->b.A);
    free(raw);

    return result;
}