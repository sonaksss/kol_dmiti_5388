#ifndef POZ_Z_D_H
#define POZ_Z_D_H

#include "numbZ.h"

/*
Автор: Еремченко Пётр
Группа: 5388
*/

/*
Остаток от деления первого целого числа на второе.

Параметры:
    - NUMBZ* a — делимое
    - NUMBZ* b — делитель

Возвращает NUMBZ* — остаток от деления a на b.
*/
NUMBZ* MOD_ZZ_Z(NUMBZ* a, NUMBZ* b);

#endif