/*
  Автор: Семенов Максим
  Группа: 5388
*/

#include "DIV_NN_Dk.h"
#include "COM_NN_D.h"
#include "MUL_Nk_N.h"
#include "MUL_ND_N.h"
#include <stdlib.h>

int DIV_NN_Dk(const NUMBN* A, const NUMBN* B) {
    if (A == NULL || B == NULL) return 0;

    if (COM_NN_D(A, B) == 1) {
        return 0;
    }

    int k = A->n - B->n;
    NUMBN* Bk = MUL_Nk_N(B, k);

    while (k >= 0 && COM_NN_D(A, Bk) == 1) {   // A < Bk
        free(Bk->A);
        free(Bk);
        if (k == 0) return 0;      
        k--;
        Bk = MUL_Nk_N(B, k);
    }
    if (k < 0) {
        if (Bk) { free(Bk->A); free(Bk); }
        return 0;
    }

    for (int q = 9; q >= 1; q--) {
        NUMBN* product = MUL_ND_N(Bk, q);      // q * Bk
        if (product == NULL) continue;

        if (COM_NN_D(product, A) != 2) {       // не "больше"
            free(Bk->A); free(Bk);
            free(product->A); free(product);
            return q;
        }
        free(product->A); free(product);
    }

    free(Bk->A); free(Bk);
    return 1;   // запасной выход
}