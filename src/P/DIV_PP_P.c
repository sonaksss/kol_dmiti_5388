/*
  Автор: Семенов Максим
  Группа: 5388
*/

#include "DIV_PP_P.h"
#include "DEG_P_N.h"
#include "LED_P_Q.h"
#include "DIV_QQ_Q.h"
#include "MUL_PQ_P.h"
#include "MUL_Pxk_P.h"
#include "SUB_PP_P.h"
#include "ADD_PP_P.h"
#include <stdlib.h>
#include <string.h>

static NUMBQ* createZeroQ() {
    NUMBQ* q = (NUMBQ*)malloc(sizeof(NUMBQ));
    q->a.b = 0;
    q->a.n = 1;
    q->a.A = (int*)malloc(sizeof(int));
    q->a.A[0] = 0;
    q->b.n = 1;
    q->b.A = (int*)malloc(sizeof(int));
    q->b.A[0] = 1;
    return q;
}

static NUMBP* createZeroP() {
    NUMBP* p = (NUMBP*)malloc(sizeof(NUMBP));
    p->m = 0;
    p->C = (NUMBQ*)malloc(sizeof(NUMBQ));
    NUMBQ* zero = createZeroQ();
    p->C[0] = *zero;
    free(zero);
    return p;
}

static NUMBP* copyP(const NUMBP* src) {
    NUMBP* dst = (NUMBP*)malloc(sizeof(NUMBP));
    dst->m = src->m;
    dst->C = (NUMBQ*)malloc((dst->m + 1) * sizeof(NUMBQ));
    for (int i = 0; i <= dst->m; i++) {
        NUMBQ srcCoef = src->C[i];
        dst->C[i].a.b = srcCoef.a.b;
        dst->C[i].a.n = srcCoef.a.n;
        dst->C[i].a.A = (int*)malloc(dst->C[i].a.n * sizeof(int));
        memcpy(dst->C[i].a.A, srcCoef.a.A, dst->C[i].a.n * sizeof(int));
        dst->C[i].b.n = srcCoef.b.n;
        dst->C[i].b.A = (int*)malloc(dst->C[i].b.n * sizeof(int));
        memcpy(dst->C[i].b.A, srcCoef.b.A, dst->C[i].b.n * sizeof(int));
    }
    return dst;
}

static void freeP(NUMBP* p) {
    if (!p) return;
    for (int i = 0; i <= p->m; i++) {
        free(p->C[i].a.A);
        free(p->C[i].b.A);
    }
    free(p->C);
    free(p);
}

NUMBP* DIV_PP_P(const NUMBP* A, const NUMBP* B) {
    if (!A || !B || (B->m == 0 && B->C[0].a.A[0] == 0)) return NULL;

    if (DEG_P_N(A) < DEG_P_N(B)) {
        return createZeroP();
    }

    NUMBP* R = copyP(A);
    NUMBP* Q = createZeroP();

    while (DEG_P_N(R) >= DEG_P_N(B)) {
        int k = DEG_P_N(R) - DEG_P_N(B);
        NUMBQ* q = DIV_QQ_Q(LED_P_Q(R), LED_P_Q(B));

        NUMBP* T = (NUMBP*)malloc(sizeof(NUMBP));
        T->m = k;
        T->C = (NUMBQ*)malloc((k+1) * sizeof(NUMBQ));
        for (int i = 0; i < k; i++) {
            NUMBQ* zero = createZeroQ();
            T->C[i] = *zero;
            free(zero);
        }
        T->C[k] = *q;
        free(q);

        NUMBP* B_shifted = MUL_Pxk_P(B, k);
        NUMBP* term = MUL_PQ_P(B_shifted, T->C[k]);
        NUMBP* newR = SUB_PP_P(R, term);

        freeP(R);
        freeP(B_shifted);
        freeP(term);
        R = newR;

        NUMBP* newQ = ADD_PP_P(Q, T);
        freeP(Q);
        freeP(T);
        Q = newQ;
    }

    freeP(R);
    return Q;
}
