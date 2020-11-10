/*
 * File:   global.c
 * Author: Administrator
 *
 * Created on November 3, 2020, 6:09 PM
 */


#include <xc.h>
#include "global.h"

 
//定义全局变量
int global_num1,global_num2,global_num3;
 
//定义函数
int func(int m  ,int n )
{
    int sum;
    sum = m+n;
    return sum;
}

//void main(void) 
//{
//    return;
//}