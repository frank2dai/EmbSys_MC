/******************************************
IC�ͺţ�TM1721-LQFB48
-------------------------------------------
 ******************/
#include "pic.h"
#define DIO  RD5
#define CLK  RD6
#define STB  RD7
#define MODE_SET 0x08                  //��ʾģʽ����
#define DISP_CON 0x97
#define DISP_WRITE  0b01000000           //��ַ�Զ�+1��ʽ
#define DISP_WRITES 0b01000100        //�̶���ַ��ʽ
/////////////////////
//bdata unsigned char y=0;      //�������λѰַȫ�ֱ��� y 
//bool y7 =y^7; 
unsigned char  ret_var;
unsigned char m,d,e,i; 
unsigned char ret_var;
unsigned char m,d,e,i; 
//////////////////////
unsigned char D9[]={0xAF,0xA0,0x6D,0xE9,0xE2,0xCB,0xC0,0xA1,0xEF,0xEB,0xE7,0xCE,0x0F,0xEC,0x8F};
unsigned char D14[]={0xAF,0xA0,0x6D,0xE9,0xE2,0xCB,0xC0,0xA1,0xEF,0xEB,0xE7,0xCE,0x0F,0xEC,0x8F};
unsigned char KEY_VAR[]={0x40,0x40,0x40,0x40,0xAF,0xAF,0xAF,0xAF,0xAF};
unsigned char  read_var;

#ifndef    _LCD_H_
        #define     _LCD_H_
         extern void DELAY(unsigned int ) ;
#endif

void TM1721_Delay_ms( unsigned int n)
{ unsigned int j;
  char k ;
    for ( j=0 ; j<n ; j++ )
    for ( k=246 ; k>0 ; k-- ) NOP ();
}
void TM1721_DELAY_US ( char n )
{ char j ;
j=n ;
while ( j>0 )
{ j-- ;
NOP (); NOP (); NOP (); NOP ();
}
}
/******************************************

******************************************/

void TM1721_Write_Byte(unsigned char dat)
{
 unsigned char i;
 TM1721_Delay_ms(2);  //?????????
 STB=0;     //???????
 for(i=0;i<8;i++)
 {
  CLK=0;
  DIO=dat&0x01;
   CLK=1;    //????????????
  dat>>=1;      
 }
STB=0;  //////
}


//------��ʾ�ӳ���-------
void xianshi(unsigned char p) 
{ 
unsigned char h; 
STB = 1; CLK = 1; DIO = 1;
TM1721_Write_Byte(0x0c);//��ʾģʽ���ã� 
STB = 1;
TM1721_Write_Byte(0x40);//д���ݵ���ʾ�Ĵ��������õ�ַ�Զ���һ 
STB = 1;
TM1721_Write_Byte(0xc0);//��ʾ�Ĵ����� 00H ��Ԫ��ʼ 
for(h=0;h<16;h++)
	{ TM1721_Write_Byte(p);//����ʾ�Ĵ��������ݣ� 
	}
STB=1; 
TM1721_Write_Byte(0x97);//��ʾ���Ʋ���������������Ļ�����ȿ���ͨ���ı����λ���� 
} 

/******************************************

******************************************/

/********************************************
��������ַ�Զ���һ��ʾ����
������
����ֵ����
********************************************/
void TM1721_Display(unsigned char p)
	{
	unsigned char h;
	STB=1;      //�˿����ó�ʼ��
 	CLK=1;
 	DIO=1;
 	TM1721_Write_Byte(MODE_SET); //��ʾģʽ
 	STB=1;
 	TM1721_Write_Byte(0x40); //Write,Addr auto+1
 	STB=1;
	TM1721_Write_Byte(0xC0); //Start Addr of the Display Register
	for (h = 0; h < 9; ++h)
		{
		TM1721_Write_Byte(p); 
		}
		STB=1;
		TM1721_Write_Byte(0x97); //Display control set
	}
/********************************************
��������ַ�Զ���һ��ʾ����
������
����ֵ����
********************************************/
void TM1721_DisplayClear(unsigned char p)
	{
	unsigned char h;
	STB=1;      //�˿����ó�ʼ��
 	CLK=1;
 	DIO=1;
 	TM1721_Write_Byte(MODE_SET); //��ʾģʽ
 	STB=1;
 	TM1721_Write_Byte(0x40); //Write,Addr auto+1
 	STB=1;
	TM1721_Write_Byte(0xC0); //Start Addr of the Display Register
	for (h = 0; h <p; ++h)
		{
		TM1721_Write_Byte(0x00); 
		}
		STB=1;
		TM1721_Write_Byte(0x97); //Display control set
	}
/********************************************
��������ַ�Զ���һ��ʾ����
������addr---��ʾ�ַ�������ʼ��ַ
   *dat---��ʾ���ݵ�����ָ��
   count---��Ҫ��ʾ�����ݵĸ���
����ֵ����
********************************************/
void TM1721_Write_String(unsigned char addr,unsigned char *dat,unsigned char count)
{
 unsigned char i;
 TM1721_Write_Byte(DISP_WRITE); 
 STB=1;
  TM1721_Write_Byte(0xc0+addr); //��ʾ�Ĵ�����00H��Ԫ��ʼ
  for(i=0;i<count;i++)
  {   
  TM1721_Write_Byte(*dat);  //����ʾ�Ĵ��������ݣ�
  dat++;
  }
  STB=1;
}
/********************************************
�������̶���ַһ��ʾ����
������ *dat---��ʾ���ݵ�����ָ��
����ֵ����
********************************************/
void TM1721_Write_StringS(unsigned char *dat)
{
  STB=1;
  TM1721_Write_Byte(DISP_WRITES); //�̶���ַ��ʽд���ݵ���ʾ�Ĵ���//0100 0100
 
  STB=1;
  TM1721_Write_Byte(0xc0);    //��ʾ�Ĵ�����00H��Ԫ��ʼ//SEG0~SEG1  //1100 0000
  TM1721_Write_Byte(dat[0]);    //����ʾ�Ĵ���д������
 
  STB=1;
  TM1721_Write_Byte(0xc1);    //��ʾ�Ĵ�����01H��Ԫ
  TM1721_Write_Byte(dat[1]);
 
  STB=1;
  TM1721_Write_Byte(0xc2);
  TM1721_Write_Byte(dat[2]);
 
  STB=1;
  TM1721_Write_Byte(0xc3);
  TM1721_Write_Byte(dat[3]);
 
  STB=1;
  TM1721_Write_Byte(0xc4);
  TM1721_Write_Byte(dat[4]);
 
  STB=1;
  TM1721_Write_Byte(0xc5);
  TM1721_Write_Byte(dat[5]);
 
  STB=1;
  TM1721_Write_Byte(0xc6);
  TM1721_Write_Byte(dat[6]);
 
  STB=1;
  TM1721_Write_Byte(0xc7);
  TM1721_Write_Byte(dat[7]);
 
  STB=1;
  TM1721_Write_Byte(0xc8);
  TM1721_Write_Byte(dat[8]);
  
  STB=1;
  TM1721_Write_Byte(0x97);    //��ʾ��//1001 0111     b7b6:10,��ʾ����;B4:0->0FF,1->ON;B3:0->1/3ƫѹ
  STB=1;
  //TM1721_Write_Byte(0xC0); //Start Addr of the Display Register
  //STB=1;
}
/*******************************************
void TM1721_Write_PWM(unsigned char addr,unsigned char dat)
{
 STB=1;
 TM1721_Write_Byte(PWM_WRITE);  //PWM?????????
 STB=1;
 TM1721_Write_Byte(0xc0+addr);  //PWM??????
 TM1721_Write_Byte(dat);
  STB=1; 
}
******************************************/

unsigned char   TM1721_Read_KeyCC(void) 
{ 
 unsigned char i; //read_var;
 //TM1721_Write_Byte(0x42);				 //���Ͷ���������
 STB=0;
 DIO=1;						 //�ͷ�DIO��������
    asm( "NOP" );
    asm( "NOP" );
    asm( "NOP" );
    asm( "NOP" );
    asm( "NOP" );
    asm( "NOP" );
   for(i=0;i<8;i++)
   {
     CLK=0;
     read_var=read_var>>1;	                        //�ӵ�λ�ȶ�
	 CLK=1;
	 if(DIO==1) 
	   read_var=read_var|0x80;
	  //TM1721_DELAY_US(2);
    asm( "NOP" );
    asm( "NOP" );
   }
    DIO=0;
    asm( "NOP" );
    asm( "NOP" );
    //read_var=0xa1;
   return (read_var);       //���ݷ���ֵ
} 



void TM1721_Key_Scan(void)
{
 //unsigned char KEY_VAR[3];  //��Ű�����ֵ����,����� 3 �ֽڵ�����,��ֵ������ 
 unsigned char i;
 STB=1;
 TM1721_Write_Byte(0x42); //������ָ���ַ�Զ���һ
 TM1721_Delay_ms(2);      
 for(i=0;i<3;i++)
 {
  KEY_VAR[i]=TM1721_Read_KeyCC(); //��ȡ������ֵ 
  TM1721_Delay_ms(2);        //ÿ����һ�ֽ�����֮��һ��С����ʱ����Ҫ�� 
 }
 switch(KEY_VAR[0]&0xFF)      // 
 {
  case 0x80:  //S1
   ret_var=0x01;break;
  case 0x40:  //S3
   ret_var=0x02;break;
  case 0x08:  //S2
   ret_var=0x04;break;
  default:
   ret_var=0x00;break; 
 }
}
/*****************************************

*****************************************/
void TM1721_Init(void)
{
 TM1721_Delay_ms(2);   //??????
 STB=1;      //???????
 CLK=1;
 DIO=1;
 TM1721_Write_Byte(MODE_SET); //��ʾģʽ��������
 STB=1;
 TM1721_Write_Byte(DISP_CON); //��ʾ��
 STB=1;
 //TM1721_Write_String(0,0x00,11);
} 


