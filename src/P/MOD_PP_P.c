/*
  Автор: Семенов Максим
  Группа: 5388
*/

#include "MOD_PP_P.h"
#include "DIV_PP_P.h"
#include "MUL_PP_P.h"
#include "SUB_PP_P.h"
#include <stdlib.h>


static void freeP(NUMBP p) {
    for (int i = 0; i <= p.m; i++) {
        free(p.C[i].a.A);
        free(p.C[i].b.A);
    }
    free(p.C);
}

NUMBP MOD_PP_P(NUMBP A, NUMBP B) {
    NUMBP Q = DIV_PP_P(A, B);
    NUMBP S = MUL_PP_P(Q, B);
    NUMBP R = SUB_PP_P(A, S);

    freeP(Q);
    freeP(S);
    return R;
}