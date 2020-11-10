/******************************************
IC型号：TM1721-LQFB48
-------------------------------------------
 ******************/
#include "pic.h"
#define DIO  RD5
#define CLK  RD6
#define STB  RD7
#define MODE_SET 0x08                  //显示模式设置
#define DISP_CON 0x97
#define DISP_WRITE  0b01000000           //地址自动+1方式
#define DISP_WRITES 0b01000100        //固定地址方式
/////////////////////
//bdata unsigned char y=0;      //定义可以位寻址全局变量 y 
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


//------显示子程序-------
void xianshi(unsigned char p) 
{ 
unsigned char h; 
STB = 1; CLK = 1; DIO = 1;
TM1721_Write_Byte(0x0c);//显示模式设置， 
STB = 1;
TM1721_Write_Byte(0x40);//写数据到显示寄存器，采用地址自动加一 
STB = 1;
TM1721_Write_Byte(0xc0);//显示寄存器的 00H 单元开始 
for(h=0;h<16;h++)
	{ TM1721_Write_Byte(p);//给显示寄存器送数据， 
	}
STB=1; 
TM1721_Write_Byte(0x97);//显示控制参数，点亮数码屏幕，亮度可以通过改变低三位调节 
} 

/******************************************

******************************************/

/********************************************
函数：地址自动加一显示函数
参数：
返回值：无
********************************************/
void TM1721_Display(unsigned char p)
	{
	unsigned char h;
	STB=1;      //端口配置初始化
 	CLK=1;
 	DIO=1;
 	TM1721_Write_Byte(MODE_SET); //显示模式
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
函数：地址自动加一显示函数
参数：
返回值：无
********************************************/
void TM1721_DisplayClear(unsigned char p)
	{
	unsigned char h;
	STB=1;      //端口配置初始化
 	CLK=1;
 	DIO=1;
 	TM1721_Write_Byte(MODE_SET); //显示模式
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
函数：地址自动加一显示函数
参数：addr---显示字符串的起始地址
   *dat---显示数据的数组指针
   count---需要显示的数据的个数
返回值：无
********************************************/
void TM1721_Write_String(unsigned char addr,unsigned char *dat,unsigned char count)
{
 unsigned char i;
 TM1721_Write_Byte(DISP_WRITE); 
 STB=1;
  TM1721_Write_Byte(0xc0+addr); //显示寄存器的00H单元开始
  for(i=0;i<count;i++)
  {   
  TM1721_Write_Byte(*dat);  //给显示寄存器送数据，
  dat++;
  }
  STB=1;
}
/********************************************
函数：固定地址一显示函数
参数： *dat---显示数据的数组指针
返回值：无
********************************************/
void TM1721_Write_StringS(unsigned char *dat)
{
  STB=1;
  TM1721_Write_Byte(DISP_WRITES); //固定地址方式写数据到显示寄存器//0100 0100
 
  STB=1;
  TM1721_Write_Byte(0xc0);    //显示寄存器的00H单元开始//SEG0~SEG1  //1100 0000
  TM1721_Write_Byte(dat[0]);    //给显示寄存器写入数据
 
  STB=1;
  TM1721_Write_Byte(0xc1);    //显示寄存器的01H单元
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
  TM1721_Write_Byte(0x97);    //显示开//1001 0111     b7b6:10,显示控制;B4:0->0FF,1->ON;B3:0->1/3偏压
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
 //TM1721_Write_Byte(0x42);				 //发送读按键命令
 STB=0;
 DIO=1;						 //释放DIO数据总线
    asm( "NOP" );
    asm( "NOP" );
    asm( "NOP" );
    asm( "NOP" );
    asm( "NOP" );
    asm( "NOP" );
   for(i=0;i<8;i++)
   {
     CLK=0;
     read_var=read_var>>1;	                        //从低位先读
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
   return (read_var);       //传递返回值
} 



void TM1721_Key_Scan(void)
{
 //unsigned char KEY_VAR[3];  //存放按键键值数组,最多存放 3 字节的数据,键值缓冲区 
 unsigned char i;
 STB=1;
 TM1721_Write_Byte(0x42); //读按键指令、地址自动加一
 TM1721_Delay_ms(2);      
 for(i=0;i<3;i++)
 {
  KEY_VAR[i]=TM1721_Read_KeyCC(); //读取按键键值 
  TM1721_Delay_ms(2);        //每读完一字节数据之后，一个小的延时、重要！ 
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
 TM1721_Write_Byte(MODE_SET); //显示模式，字配置
 STB=1;
 TM1721_Write_Byte(DISP_CON); //显示开
 STB=1;
 //TM1721_Write_String(0,0x00,11);
} 


