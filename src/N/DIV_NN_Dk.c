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
    if (k < 0) {
        free(Bk.A);
        return 0;
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
