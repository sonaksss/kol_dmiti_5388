/*
    SUB_ZZ_Z – вычитание целых чисел.

    Выполняет A - B.
    Цифры хранятся от младшего разряда к старшему (little-endian).

    Параметры:
        1) const NUMBZ* A – уменьшаемое
        2) const NUMBZ* B – вычитаемое

    Возвращает: NUMBZ* – разность, или NULL при ошибке

    Автор: Семенов Максим
    Группа: 5388
*/
#ifndef _SUB_ZZ_Z_H_
#define _SUB_ZZ_Z_H_

#include "numbZ.h"

NUMBZ* SUB_ZZ_Z(const NUMBZ* A, const NUMBZ* B);

#endif