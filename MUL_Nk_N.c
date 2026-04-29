#include "MUL_Nk_N.h"
#include <stdlib.h>

NUMBN MUL_NK_N(const NUMBN* num, int k){
    /*
    mulNk – умножение натурального числа на 10^k
    
    Умножает длинное натуральное число на 10 в степени k.
    Цифры хранятся от старшего разряда к младшему (big-endian).
    
    Параметры:
        1) const NUMBN* num – исходное натуральное число
        2) int k – натуральное или 0
    
    Возвращает: NUMBN – результат умножения (num * 10^k)
    
    Автор: Семенов Максим
    Группа: 5388
    */
    
    NUMBN result;
    result.n = num->n + k;
    result.A = (int*)malloc(result.n * sizeof(int));
    if(result.A == NULL){
        result.n = 0;
        return result;
    }
    for(int i=0; i<result.n; i++){
        if(i < num->n){
            result.A[i] = num->A[i];
        }
        else{
            result.A[i] = 0;
        }
    }
    return result;
}