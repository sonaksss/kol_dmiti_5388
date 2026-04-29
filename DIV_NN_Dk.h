#ifndef _DIV_NN_DK_H_
#define _DIV_NN_DK_H_

#include "numbN.h"

/*
   DIV_NN_Dk – первая цифра частного от деления A на B.

   Находит такую цифру q (1..9), что
   q * (B * 10^k) <= A < (q+1) * (B * 10^k),
   где k подбирается автоматически.
 
   Параметры:
     1) const NUMBN* A – делимое (натуральное)
     2) const NUMBN* B – делитель (натуральное, > 0)
  
   Возвращает: int – цифра q (0..9), 0 если A < B.

   Автор: Семенов Максим
   Группа: 5388
*/
int DIV_NN_Dk(const NUMBN* A, const NUMBN* B);

#endif