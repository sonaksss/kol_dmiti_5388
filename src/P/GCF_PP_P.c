#include "GCF_PP_P.h"
#include "DEG_P_N.h"
#include "MOD_PP_P.h"
#include "POZ_Z_D.h"
#include <stdlib.h>

/*
Автор: Еремченко Пётр
Группа: 5388
*/

static int is_zero_p(NUMBP* p) {
    if (!p || !p->C) return 1;
    return (p->m == 0 && POZ_Z_D(&(p->C[0].a)) == 0);
}

static void free_polynom(NUMBP* p) {
    if (!p) return;
    if (p->C) {
        for (int i = 0; i <= p->m; i++) {
            free(p->C[i].a.A);
            free(p->C[i].b.A);
        }
        free(p->C);
    }
    free(p);
}

static NUMBP* copy_polynom(NUMBP* src) {
    if (!src || !src->C) return NULL;
    
    NUMBP* dst = (NUMBP*)malloc(sizeof(NUMBP));
    if (!dst) return NULL;
    
    dst->m = src->m;
    dst->C = (NUMBQ*)malloc((src->m + 1) * sizeof(NUMBQ));
    if (!dst->C) {
        free(dst);
        return NULL;
    }
    
    for (int i = 0; i <= src->m; i++) {
        NUMBQ* copy = copy_q(&(src->C[i]));
        if (!copy) {
            for (int j = 0; j < i; j++) {
                free(dst->C[j].a.A);
                free(dst->C[j].b.A);
            }
            free(dst->C);
            free(dst);
            return NULL;
        }
        dst->C[i] = *copy;
        free(copy);
    }
    
    return dst;
}

/*
НОД многочленов (алгоритм Евклида).
  
Принимает 2 многочлена NUMBP*

Возвращает NUMBP* — НОД многочленов.
В случае ошибки возвращает NULL.
*/
NUMBP* GCF_PP_P(NUMBP* a, NUMBP* b) {
    if (!a || !b || !a->C || !b->C) return NULL;
    
    NUMBP* temp_a = copy_polynom(a);
    NUMBP* temp_b = copy_polynom(b);
    
    if (!temp_a || !temp_b) {
        free_polynom(temp_a);
        free_polynom(temp_b);
        return NULL;
    }
    
    while (!is_zero_p(temp_a) && !is_zero_p(temp_b)) {
        int deg_a = DEG_P_N(temp_a);
        int deg_b = DEG_P_N(temp_b);
        
        if (deg_a >= deg_b) {
            NUMBP* new_a = MOD_PP_P(temp_a, temp_b);
            if (!new_a) {
                free_polynom(temp_a);
                free_polynom(temp_b);
                return NULL;
            }
            free_polynom(temp_a);
            temp_a = new_a;
        } else {
            NUMBP* new_b = MOD_PP_P(temp_b, temp_a);
            if (!new_b) {
                free_polynom(temp_a);
                free_polynom(temp_b);
                return NULL;
            }
            free_polynom(temp_b);
            temp_b = new_b;
        }
    }
    
    NUMBP* result = is_zero_p(temp_a) ? temp_b : temp_a;
    free_polynom(is_zero_p(temp_a) ? temp_a : temp_b);
    
    return result;
}