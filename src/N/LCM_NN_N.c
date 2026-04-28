#include "LCM_NN_N.h"
#include "MUL_NN_N.h"
#include <stdlib.h>

/*
  Автор: Александров Ярослав
  Группа: 5388
*/

/*
  Примечание по структуре NUMBN:
    - A[0]   — старшая цифра числа
    - A[n-1] — младшая цифра числа
    - n      — количество цифр в числе
*/

/* ─── Вспомогательные функции ─── */

static NUMBN* allocNatural(int size) {
    NUMBN* r = (NUMBN*)malloc(sizeof(NUMBN));
    r->n = size;
    r->A = (int*)calloc(size, sizeof(int));
    return r;
}

static void removeLeadingZeros(NUMBN* num) {
    int leading = 0;
    while (leading < num->n - 1 && num->A[leading] == 0) leading++;
    if (leading > 0) {
        int newN = num->n - leading;
        int* newA = (int*)malloc(newN * sizeof(int));
        for (int i = 0; i < newN; i++) newA[i] = num->A[i + leading];
        free(num->A);
        num->A = newA;
        num->n = newN;
    }
}

static NUMBN* copyNatural(NUMBN* src) {
    NUMBN* copy = allocNatural(src->n);
    for (int i = 0; i < src->n; i++) copy->A[i] = src->A[i];
    return copy;
}

static int isZero(NUMBN* num) {
    return num->n == 1 && num->A[0] == 0;
}

/* ─── Заглушки зависимых модулей ─── */

/*
  COM_NN_D_stub

  Заглушка модуля COM_NN_D.
  Сравнение двух натуральных чисел.
  При интеграции: удалить эту функцию и подключить #include "COM_NN_D.h",
  заменить все вызовы COM_NN_D_stub(...) на compareNaturalNumbers(...).

  Параметры:
    1) NUMBN* a - первое число
    2) NUMBN* b - второе число

  Возвращает int: 2 - a > b, 0 - a == b, 1 - a < b
*/
static int COM_NN_D_stub(NUMBN* a, NUMBN* b) {
    if (a->n != b->n) return (a->n > b->n) ? 2 : 1;
    for (int i = 0; i < a->n; i++) {
        if (a->A[i] != b->A[i]) return (a->A[i] > b->A[i]) ? 2 : 1;
    }
    return 0;
}

/*
  SUB_NN_N_stub

  Заглушка модуля SUB_NN_N.
  Вычитание из первого большего натурального числа второго меньшего или равного.
  При интеграции: удалить эту функцию и подключить #include "SUB_NN_N.h",
  заменить все вызовы SUB_NN_N_stub(...) на subtractNaturalFromNatural(...).

  Параметры:
    1) NUMBN* a - уменьшаемое (a >= b)
    2) NUMBN* b - вычитаемое

  Возвращает указатель на новое NUMBN = a - b
*/
static NUMBN* SUB_NN_N_stub(NUMBN* a, NUMBN* b) {
    NUMBN* result = allocNatural(a->n);
    int borrow = 0;
    for (int i = 0; i < a->n; i++) {
        int aDigit = a->A[a->n - 1 - i];
        int bDigit = (i < b->n) ? b->A[b->n - 1 - i] : 0;
        int diff = aDigit - bDigit - borrow;
        if (diff < 0) { diff += 10; borrow = 1; }
        else          { borrow = 0; }
        result->A[result->n - 1 - i] = diff;
    }
    removeLeadingZeros(result);
    return result;
}

/*
  GCF_NN_N_stub

  Заглушка модуля GCF_NN_N.
  НОД двух натуральных чисел (алгоритм Евклида).
  При интеграции: удалить эту функцию и подключить #include "GCF_NN_N.h",
  заменить все вызовы GCF_NN_N_stub(...) на greatestCommonDivisor(...).

  Параметры:
    1) NUMBN* a - первое число
    2) NUMBN* b - второе число

  Возвращает указатель на новое NUMBN = НОД(a, b)
*/
static NUMBN* GCF_NN_N_stub(NUMBN* a, NUMBN* b) {
    NUMBN* x = copyNatural(a);
    NUMBN* y = copyNatural(b);

    while (!isZero(y)) {
        NUMBN* r = copyNatural(x);
        while (COM_NN_D_stub(r, y) != 1) {
            NUMBN* tmp = SUB_NN_N_stub(r, y);
            free(r->A); free(r);
            r = tmp;
        }
        free(x->A); free(x);
        x = y;
        y = r;
    }

    free(y->A); free(y);
    return x;
}

/*
  DIV_NN_N_stub

  Заглушка модуля DIV_NN_N.
  Целочисленное деление a на b.
  При интеграции: удалить эту функцию и подключить #include "DIV_NN_N.h",
  заменить все вызовы DIV_NN_N_stub(...) на divideNaturalByNatural(...).

  Параметры:
    1) NUMBN* a - делимое
    2) NUMBN* b - делитель (не ноль)

  Возвращает указатель на новое NUMBN = a / b (целая часть)
*/
static NUMBN* DIV_NN_N_stub(NUMBN* a, NUMBN* b) {
    NUMBN* quot = allocNatural(1);
    NUMBN* rem  = copyNatural(a);

    while (COM_NN_D_stub(rem, b) != 1) {
        NUMBN* tmp = SUB_NN_N_stub(rem, b);
        free(rem->A); free(rem);
        rem = tmp;

        /* quot++ */
        int carry = 1;
        for (int i = quot->n - 1; i >= 0 && carry; i--) {
            int s = quot->A[i] + carry;
            quot->A[i] = s % 10;
            carry = s / 10;
        }
        if (carry) {
            int* newA = (int*)calloc(quot->n + 1, sizeof(int));
            newA[0] = carry;
            for (int i = 0; i < quot->n; i++) newA[i + 1] = quot->A[i];
            free(quot->A);
            quot->A = newA;
            quot->n++;
        }
    }

    free(rem->A); free(rem);
    return quot;
}

/* ─── Основная функция ─── */

/*
  leastCommonMultiple

  НОК двух натуральных чисел по формуле: НОК(a, b) = (a / НОД(a, b)) * b.
  Деление выполняется до умножения, чтобы работать с меньшими числами.
  Использует GCF_NN_N_stub, DIV_NN_N_stub и multiplyNaturalByNatural (MUL_NN_N).

  Параметры:
    1) NUMBN* a - первое натуральное число
    2) NUMBN* b - второе натуральное число

  Возвращает указатель на новое NUMBN = НОК(a, b)
*/
NUMBN* leastCommonMultiple(NUMBN* a, NUMBN* b) {
    NUMBN* gcd    = GCF_NN_N_stub(a, b);
    NUMBN* aOverG = DIV_NN_N_stub(a, gcd);
    free(gcd->A); free(gcd);

    NUMBN* result = multiplyNaturalByNatural(aOverG, b);
    free(aOverG->A); free(aOverG);
    return result;
}
