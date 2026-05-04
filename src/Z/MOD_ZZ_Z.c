#include <stdlib.h>
#include "MOD_ZZ_Z.h"
#include "POZ_Z_D.h"
#include "DIV_ZZ_Z.h"
#include "MUL_ZZ_Z.h"
#include "SUB_ZZ_Z.h"

/*
Автор: Еремченко Пётр
Группа: 5388
*/

/*
  Освобождает память, занятую структурой NUMBZ.

  Параметры:
    - NUMBZ* num — число для освобождения.

  Ничего не возвращает.
*/
static void freeNumbZ(NUMBZ* num) {
    if (!num) return;
    free(num->A);
    free(num);
}

/*
Остаток от деления первого целого числа на второе.

Параметры:
    - NUMBZ* a — делимое
    - NUMBZ* b — делитель

Возвращает NUMBZ* — остаток от деления a на b.
*/
NUMBZ* MOD_ZZ_Z(NUMBZ* a, NUMBZ* b){
    if (POZ_Z_D(b) == 0) {
        return NULL;
    }

    NUMBZ* quotient = DIV_ZZ_Z(a, b);
    NUMBZ* product = MUL_ZZ_Z(quotient, b);
    NUMBZ* result = SUB_ZZ_Z(a, product);
    
    freeNumbZ(quotient);
    freeNumbZ(product);
    
    return result;
}