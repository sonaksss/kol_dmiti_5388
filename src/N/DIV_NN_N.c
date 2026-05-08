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

// Создаёт копию натурального числа
static NUMBN* copyN(const NUMBN* src) {
    if (!src) return NULL;
    NUMBN* dst = (NUMBN*)malloc(sizeof(NUMBN));
    if (!dst) return NULL;
    dst->n = src->n;
    dst->A = (int*)malloc(dst->n * sizeof(int));
    if (!dst->A) { free(dst); return NULL; }
    for (int i = 0; i < src->n; i++) dst->A[i] = src->A[i];
    return dst;
}

NUMBN* DIV_NN_N(const NUMBN* A, const NUMBN* B) {
    if (!A || !B) return NULL;

    // Создаём 0
    NUMBN* zero = (NUMBN*)malloc(sizeof(NUMBN));
    zero->n = 1;
    zero->A = (int*)calloc(1, sizeof(int));
    zero->A[0] = 0;

    if (COM_NN_D(A, B) == 1) return zero;  // A < B

    NUMBN* R = copyN(A);   // остаток
    NUMBN* Q = copyN(zero); // частное

    while (COM_NN_D(R, B) != 1) {   // R >= B
        // Определяем сдвиг k
        int k = R->n - B->n;
        NUMBN* Bk = MUL_Nk_N(B, k);
        while (k >= 0 && COM_NN_D(R, Bk) == 1) {  // R < Bk
            free(Bk->A); free(Bk);
            k--;
            if (k >= 0) Bk = MUL_Nk_N(B, k);
            else { Bk = NULL; break; }
        }
        if (k < 0 || !Bk) break;

        // Цифра q
        int q = DIV_NN_Dk(R, B);
        free(Bk->A); free(Bk);

        // Вычитаем q * (B * 10^k)
        NUMBN* shiftedB = MUL_Nk_N(B, k);
        NUMBN* newR = SUB_NDN_N(R, shiftedB, q);
        free(R->A); free(R);
        free(shiftedB->A); free(shiftedB);
        R = newR;

        // Добавляем q * 10^k к Q
        NUMBN* qNum = (NUMBN*)malloc(sizeof(NUMBN));
        qNum->n = 1;
        qNum->A = (int*)malloc(sizeof(int));
        qNum->A[0] = q;

        NUMBN* addTerm = MUL_Nk_N(qNum, k);
        free(qNum->A); free(qNum);

        NUMBN* newQ = ADD_NN_N(Q, addTerm);
        free(Q->A); free(Q);
        free(addTerm->A); free(addTerm);
        Q = newQ;
    }

    free(R->A); free(R);
    free(zero->A); free(zero);
    return Q;
}