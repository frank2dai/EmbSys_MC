/* 
 * File:   AD.h
 * Author: Administrator
 *
 * Created on November 2, 2020, 10:41 AM
 */
#include <stdlib.h>
#include <xc.h>
////////////宏及变量定义//////////////////
#ifndef    _LCD_H_
    #define     _LCD_H_
      void DELAY(unsigned int ) ;
#endif


extern  int   caiji_val ;

//------------------------------------------------


////////////ADC采集函数/////////////////
void adcinit(void)
{   int	AD,i;
    //caiji_val=0;		      //采集单元清零
    ANSELA=0X0F;
    TRISA=1;		      //设置A3为输入
    //PORTA=0X0F;	      //错！1=端口引脚电平>VIH,写 PORTA写的是相应LATA 寄存器
	ADCON0=0x0D;             //选择AN3通道，并使能ADC
	ADCON1=0x10;		      //选择左对齐方式,ADC时钟设为FOSC/8,
	                               //VREF-与VSS链接，VREF+与VDD链接，选择VDD作为参考电压
	                               //如需用其他电压作为参考电压修改ADCON1寄存器后3位就行
    //for(i=0;i<30;i++)
    //{NOP();}  
    DELAY(20);
    //while(1);          //等待转换完成
    GO = 1;
    while(GO); //GO还可替换成：nDONE、GO_DONE等
    ADIF=0;
    caiji_val= ADRESH;
    caiji_val=(caiji_val<<2)+( ADRESL>>6);
    AD=(int)(caiji_val*0.489);	  //0.489为比例系数,1023*0.489=5.00V
}


