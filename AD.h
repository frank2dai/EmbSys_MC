/* 
 * File:   AD.h
 * Author: Administrator
 *
 * Created on November 2, 2020, 10:41 AM
 */
#include <stdlib.h>
#include <xc.h>
////////////�꼰��������//////////////////
#ifndef    _LCD_H_
    #define     _LCD_H_
      void DELAY(unsigned int ) ;
#endif


extern  int   caiji_val ;

//------------------------------------------------


////////////ADC�ɼ�����/////////////////
void adcinit(void)
{   int	AD,i;
    //caiji_val=0;		      //�ɼ���Ԫ����
    ANSELA=0X0F;
    TRISA=1;		      //����A3Ϊ����
    //PORTA=0X0F;	      //��1=�˿����ŵ�ƽ>VIH,д PORTAд������ӦLATA �Ĵ���
	ADCON0=0x0D;             //ѡ��AN3ͨ������ʹ��ADC
	ADCON1=0x10;		      //ѡ������뷽ʽ,ADCʱ����ΪFOSC/8,
	                               //VREF-��VSS���ӣ�VREF+��VDD���ӣ�ѡ��VDD��Ϊ�ο���ѹ
	                               //������������ѹ��Ϊ�ο���ѹ�޸�ADCON1�Ĵ�����3λ����
    //for(i=0;i<30;i++)
    //{NOP();}  
    DELAY(20);
    //while(1);          //�ȴ�ת�����
    GO = 1;
    while(GO); //GO�����滻�ɣ�nDONE��GO_DONE��
    ADIF=0;
    caiji_val= ADRESH;
    caiji_val=(caiji_val<<2)+( ADRESL>>6);
    AD=(int)(caiji_val*0.489);	  //0.489Ϊ����ϵ��,1023*0.489=5.00V
}


