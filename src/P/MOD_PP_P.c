/*
  Автор: Семенов Максим
  Группа: 5388
*/

#include "MOD_PP_P.h"
#include "DIV_PP_P.h"
#include "MUL_PP_P.h"
#include "SUB_PP_P.h"
#include <stdlib.h>

NUMBP* MOD_PP_P(const NUMBP* A, const NUMBP* B) {
    if (!A || !B) return NULL;

    NUMBP* Q = DIV_PP_P(A, B);
    if (!Q) return NULL;

    NUMBP* S = MUL_PP_P(Q, B);
    if (!S) { freeP(Q); return NULL; }

    NUMBP* R = SUB_PP_P(A, S);

    freeP(Q);
    freeP(S);
    return R;
}