#include "GCF_NN_N.h"
#include "NZER_N_B.h"
#include "MOD_NN_N.h"
#include "COM_NN_D.h"

/*
Автор: Еремченко Пётр
Группа: 5388
*/

/*
НОД натуральных чисел

Принимает 2 натуральных числа
Вычисляет НОД и возвращет натуральное число
*/
NUMBN GCF_NN_N(NUMBN a, NUMBN b){
    NUMBN temp_a = a;
    NUMBN temp_b = b;

    while (NZER_N_B(temp_a) && NZER_N_B(temp_b)) {
        if (COM_NN_D(temp_a, temp_b) == 2) {
            temp_a = MOD_NN_N(temp_a, temp_b);
        } else {
            temp_b = MOD_NN_N(temp_b, temp_a);
        }
    }

    return NZER_N_B(temp_a) ? temp_a : temp_b;
}