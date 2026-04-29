#include "DIV_NN_Dk.h"
#include "COM_NN_D.h"
#include "MUL_Nk_N.h"
#include "MUL_ND_N.h"
#include <stdlib.h>

/*
    DIV_NN_Dk – первая цифра частного от деления большего натурального на меньшее,
    домноженное на 10^k, где k – номер позиции этой цифры (номер считается с нуля).

    Функция находит такую десятичную цифру q (0..9), что
    q * (B * 10^k) <= A < (q+1) * (B * 10^k),
    где k подбирается автоматически как разность длин чисел.

    Цифры хранятся от старшего разряда к младшему.

    Параметры:
        1) const NUMBN* A – делимое (натуральное число)
        2) const NUMBN* B – делитель (натуральное число, > 0)

    Возвращает: int – цифра q (0, если A < B, иначе 1..9)

    Автор: Семенов Максим
    Группа: 5388
*/

int DIV_NN_Dk(const NUMBN* A, const NUMBN* B) {
    if (COM_NN_D(A, B) == 1) {
        return 0;
    }
    int k = A->n - B->n;
    NUMBN Bk = MUL_Nk_N(B, k);

    while (k >= 0 && COM_NN_D(A, &Bk) == 1) {  
        free(Bk.A);
        if (k == 0) {
            return 0;
        }
        k--;
        Bk = MUL_Nk_N(B, k);
    }

    for (int q = 9; q >= 1; q--) {
        NUMBN product = MUL_ND_N(Bk, q);
        if (COM_NN_D(&product, A) != 2) {   
            free(Bk.A);
            free(product.A);
            return q;
        }
        free(product.A);
    }

    free(Bk.A);
    return 1;
}