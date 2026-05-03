/*
  Автор: Семенов Максим
  Группа: 5388
*/

#include "DIV_NN_N.h"
#include "DIV_NN_Dk.h"
#include "MUL_Nk_N.h"
#include "MUL_ND_N.h"
#include "COM_NN_D.h"
#include "SUB_NDN_N.h"
#include "ADD_NN_N.h"    
#include <stdlib.h>

NUMBN DIV_NN_N(const NUMBN* A, const NUMBN* B) {
    NUMBN zero;
    zero.n = 1;
    zero.A = (int*)calloc(1, sizeof(int));
    zero.A[0] = 0;

    if (COM_NN_D(A, B) == 1) {  
        return zero;
    }

    NUMBN R;
    R.n = A->n;
    R.A = (int*)malloc(R.n * sizeof(int));
    for (int i = 0; i < R.n; i++) {
        R.A[i] = A->A[i];
    }

    NUMBN Q;
    Q.n = 1;
    Q.A = (int*)malloc(sizeof(int));
    Q.A[0] = 0;

    while (COM_NN_D(&R, B) != 1) {
        int k = R.n - B->n;
        NUMBN Bk = MUL_Nk_N(B, k);

        while (k >= 0 && COM_NN_D(&R, &Bk) == 1) {  // R < Bk
            free(Bk.A);
            k--;
            if (k >= 0) Bk = MUL_Nk_N(B, k);
        }

        if (k < 0) break;
        int q = DIV_NN_Dk(&R, B);
        free(Bk.A);
        NUMBN shiftedB = MUL_Nk_N(B, k);
        NUMBN newR = SUB_NDN_N(R, shiftedB, q);
        free(R.A);
        free(shiftedB.A);
        R = newR;
        NUMBN qNum;
        qNum.n = 1;
        qNum.A = (int*)malloc(sizeof(int));
        qNum.A[0] = q;

        NUMBN addTerm = MUL_Nk_N(&qNum, k);
        free(qNum.A);

        NUMBN newQ = ADD_NN_N(Q, addTerm);
        free(Q.A);
        free(addTerm.A);
        Q = newQ;
    }

    free(R.A);
    return Q;
}
