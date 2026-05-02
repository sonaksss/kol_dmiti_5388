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

static NUMBP createZeroP() {
    NUMBP zero;
    zero.m = 0;
    zero.C = (NUMBQ*)malloc(sizeof(NUMBQ));
    zero.C[0].a.b = 0;
    zero.C[0].a.n = 1;
    zero.C[0].a.A = (int*)malloc(sizeof(int));
    zero.C[0].a.A[0] = 0;
    zero.C[0].b.n = 1;
    zero.C[0].b.A = (int*)malloc(sizeof(int));
    zero.C[0].b.A[0] = 1;
    return zero;
}


static NUMBP copyP(NUMBP src) {
    NUMBP dst;
    dst.m = src.m;
    dst.C = (NUMBQ*)malloc((dst.m + 1) * sizeof(NUMBQ));
    for (int i = 0; i <= dst.m; i++) {
        // копируем числитель
        dst.C[i].a.b = src.C[i].a.b;
        dst.C[i].a.n = src.C[i].a.n;
        dst.C[i].a.A = (int*)malloc(dst.C[i].a.n * sizeof(int));
        memcpy(dst.C[i].a.A, src.C[i].a.A, dst.C[i].a.n * sizeof(int));
        // копируем знаменатель
        dst.C[i].b.n = src.C[i].b.n;
        dst.C[i].b.A = (int*)malloc(dst.C[i].b.n * sizeof(int));
        memcpy(dst.C[i].b.A, src.C[i].b.A, dst.C[i].b.n * sizeof(int));
    }
    return dst;
}

static void freeP(NUMBP p) {
    for (int i = 0; i <= p.m; i++) {
        free(p.C[i].a.A);
        free(p.C[i].b.A);
    }
    free(p.C);
}

NUMBP DIV_PP_P(NUMBP A, NUMBP B) {
    if (DEG_P_N(A) < DEG_P_N(B)) {   
        return createZeroP();
    }

    NUMBP R = copyP(A);         
    NUMBP Q = createZeroP();   

    while (DEG_P_N(R) >= DEG_P_N(B)) {
        int k = DEG_P_N(R) - DEG_P_N(B);
        NUMBQ q = DIV_QQ_Q(LED_P_Q(R), LED_P_Q(B));   

        NUMBP T;
        T.m = k;
        T.C = (NUMBQ*)malloc((k + 1) * sizeof(NUMBQ));
        for (int i = 0; i < k; i++) {
            T.C[i].a.b = 0;
            T.C[i].a.n = 1;
            T.C[i].a.A = (int*)malloc(sizeof(int));
            T.C[i].a.A[0] = 0;
            T.C[i].b.n = 1;
            T.C[i].b.A = (int*)malloc(sizeof(int));
            T.C[i].b.A[0] = 1;
        }
        T.C[k] = q;  
        NUMBP B_shifted = MUL_Pxk_P(B, k);
        NUMBP term = MUL_PQ_P(B_shifted, q);    
        NUMBP newR = SUB_PP_P(R, term);

        freeP(R);
        freeP(B_shifted);
        freeP(term);
        R = newR;


        NUMBP newQ = ADD_PP_P(Q, T);
        freeP(Q);
        freeP(T);
        Q = newQ;
    }

    freeP(R);
    return Q;
}