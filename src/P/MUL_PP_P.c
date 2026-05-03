/*
  Автор: Семенов Максим
  Группа: 5388
*/

#include "MUL_PP_P.h"
#include "ADD_QQ_Q.h"
#include "MUL_QQ_Q.h"
#include <stdlib.h>

NUMBP MUL_PP_P(NUMBP A, NUMBP B) {
    NUMBP result;
    result.m = A.m + B.m;
    result.C = (NUMBQ*)malloc((result.m + 1) * sizeof(NUMBQ));

    for (int i = 0; i <= result.m; i++) {
        result.C[i].a.b = 0;                
        result.C[i].a.n = 1;                
        result.C[i].a.A = (int*)malloc(sizeof(int));
        result.C[i].a.A[0] = 0;
        result.C[i].b.n = 1;
        result.C[i].b.A = (int*)malloc(sizeof(int));
        result.C[i].b.A[0] = 1;
    }

    for (int i = 0; i <= A.m; i++) {
        for (int j = 0; j <= B.m; j++) {
            NUMBQ term = MUL_QQ_Q(A.C[i], B.C[j]); 
            NUMBQ sum = ADD_QQ_Q(result.C[i + j], term); 
            free(result.C[i + j].a.A);
            free(result.C[i + j].b.A);
            free(term.a.A);
            free(term.b.A);
            result.C[i + j] = sum;  
        }
    }
    return result;
}