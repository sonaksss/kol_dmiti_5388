/*
  Автор: Семенов Максим
  Группа: 5388
*/

#include "MUL_PP_P.h"
#include "ADD_QQ_Q.h"
#include "MUL_QQ_Q.h"
#include <stdlib.h>

// Создание нулевого коэффициента (0/1)
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

static void freeQ(NUMBQ* q) {
    if (!q) return;
    free(q->a.A);
    free(q->b.A);
    free(q);
}

NUMBP* MUL_PP_P(const NUMBP* A, const NUMBP* B) {
    if (!A || !B) return NULL;

    NUMBP* result = (NUMBP*)malloc(sizeof(NUMBP));
    if (!result) return NULL;
    result->m = A->m + B->m;
    result->C = (NUMBQ*)malloc((result->m + 1) * sizeof(NUMBQ));
    if (!result->C) { free(result); return NULL; }

    // Инициализация нулями
    for (int i = 0; i <= result->m; i++) {
        NUMBQ* zero = createZeroQ();
        result->C[i] = *zero;   // копируем содержимое
        free(zero);             // освобождаем обёртку
    }

    // Свёртка
    for (int i = 0; i <= A->m; i++) {
        for (int j = 0; j <= B->m; j++) {
            NUMBQ* term = MUL_QQ_Q(&A->C[i], &B->C[j]);
            NUMBQ* sum = ADD_QQ_Q(&result->C[i + j], term);
            // Освобождаем старый коэффициент в result
            free(result->C[i + j].a.A);
            free(result->C[i + j].b.A);
            result->C[i + j] = *sum;   // копируем содержимое
            free(term->a.A); free(term->b.A); free(term);
            free(sum->a.A); free(sum->b.A); free(sum);
        }
    }
    return result;
}