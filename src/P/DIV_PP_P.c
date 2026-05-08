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

// Вспомогательные функции createZeroP, copyP, freeP аналогичны предыдущим.

NUMBP* DIV_PP_P(const NUMBP* A, const NUMBP* B) {
    if (!A || !B || (B->m == 0 && B->C[0].a.A[0] == 0)) return NULL; // деление на 0

    if (DEG_P_N(A) < DEG_P_N(B)) {
        return createZeroP();
    }

    NUMBP* R = copyP(A);
    NUMBP* Q = createZeroP();

    while (DEG_P_N(R) >= DEG_P_N(B)) {
        int k = DEG_P_N(R) - DEG_P_N(B);
        NUMBQ* q = DIV_QQ_Q(LED_P_Q(R), LED_P_Q(B));

        // Создаём одночлен T = q * x^k
        NUMBP* T = (NUMBP*)malloc(sizeof(NUMBP));
        T->m = k;
        T->C = (NUMBQ*)malloc((k+1) * sizeof(NUMBQ));
        for (int i = 0; i < k; i++) {
            T->C[i] = *createZeroQ();  // копируем нуль
        }
        T->C[k] = *q;   // старший коэффициент
        free(q);        // q больше не нужен как указатель, содержимое скопировано

        NUMBP* B_shifted = MUL_Pxk_P(B, k);
        NUMBP* term = MUL_PQ_P(B_shifted, T->C[k]);  // q * (B * x^k)
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