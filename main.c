#include <pic.h>
#include <stdlib.h>
#include <xc.h>
#include <string.h> /* for memchr, NULL */
#include <stdio.h> /* for printf */
#include "mcc_generated_files/mcc.h"
#include "TM1721.h"
#include "AD.h"


//#define LED1 PD1
//#define LED5 PD5
#define led_red  RD4
#define INT8U   unsigned char
#define INT16U  unsigned int
#define uchar   unsigned char
#define uint  unsigned int

#define _XTAL_FREQ 4000000UL
#define T0_10ms 0x64;
INT8U *p,WFLAG=1,tempM60=0,TimeFLAG=0,Intrcnt=0,m=0;
INT16U temp1=0,temp2=0,tempM0=0,tempM=0,temph=0,tempH=995,tempHD;
INT16U Operation_Type=1,Time_Count=0,countM=0,KeyValue;
//__bit TimeFLAG = false ,WFLAG ;
//int *p;                                                     //①固定地址模式显示0 1 2 3 4
//int dat[]={0xAF,0xA0,0x6D,0xE9,0xE2,0xCB,0xCF,0xA1,0xEF};   //①
unsigned char SEG_CODE[]={0xAF,0xA0,0x6D,0xE9,0xE2,0xCB,0xCF,0xA1,0xEF,0xEB}; //0-9
//unsigned char dat[]={0xEF,0xEB,0xE7,0xCE,0x0F,0xEC,0x4F,0x47,0xEF}; //8-F
unsigned char dat[]={0xAF,0xAF,0xAF,0xAF,0xAF,0xAF,0xAF,0xAF,0xE2}; //0-8
unsigned char Key_arr[]={0x00,0x00,0x00};
//EEPROM
//__EEPROM_DATA(0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77);  
int     caiji_val=3;
//o2__UART
uchar receiveo2[12];
uchar sendo2[4]={0x11,0x01,0x01,0xED};
uchar receive_count=0;
uchar send_count=0;
uchar *p;
uchar SciReceiveFlag=0;
uchar SciSendFlag=0;
/*****************************
 * 
 */
void sciinitial()
{
 TXSTA=0X04;           //8位数据，异步模式，高速。
 RCSTA=0X80;           //使能串行通信，8位接收。
 TRISG2=1;     			//(RX)设置为输入方式
 TRISG1=0;     			//(TX)设置为输出方式
 SPBRG=25;             //波特率设置，fosc/16(x+1).
 PIR1=0;               //清中断标志
 PIE1=PIE1|0x20;        //允许接收中断。RCIE,TXIE
 PIE1=PIE1|0X10;
 RC2STAbits.CREN=1;               //允许串行口连续接收数据
 TX2STAbits.TXEN=1;               //允许串行口发送数据
}
// ====== ??( n )ms
 void DELAY(unsigned int n)
{ unsigned int j;
  char k ;
    for ( j=0 ; j<n ; j++ )
    for ( k=246 ; k>0 ; k-- ) NOP ();
}

// ====== ????S
void delay(uint t1)
{
    uint i,j;
    for(i=1;i<t1;i++)
    {
        for(j=1;j<100;j++);
    }  
}
// ====== ??(n ) ms ,????
void DELAY_I( unsigned int n )
{ unsigned int j ;
char k ;
for ( j=0 ; j<n ; j++ )
for ( k=246 ; k>0 ; k-- ) NOP ();
}

// ====== ??(n � ) ? s+12 ? s ,??,?????????
void DELAY_US ( char n )
{ char j ;
j=n ;
while ( j>0 )
{ j-- ;
NOP (); NOP (); NOP (); NOP ();
}
}

void Init(void)
{
    // initialize the device
    //ADCON1 = 0x06;
PORTA = TRISA = 0x00;                  //PB??????,???0x00
PORTB = TRISB = 0x00;                  //PB??????,???0x00
PORTF = TRISF = 0x00;                  //PB??????,???0x00

    TRISC=0XFA;
    TRISD=0X00;
    TRISE=0XC0;//RE output


    OSCCON= 0x68;                         //?????4MHz
    ANSELA=0X04;                         //PORTA ??????? 0??
    ADCON0=0x0C;             //选择AN3通道，并使能ADC
    ADCON1=0x10;		      //选择左对齐方式,ADC时钟设为FOSC/8,
                              //VREF-与VSS链接，VREF+与VDD链接，选择VDD作为参考电压
                              //如需用其他电压作为参考电压修改ADCON1寄存器后3位就行
    

    
    ////nWPUEN=1;
    //WPUA=0X00;
    //WPUB=0X21;
    //WPUC=0XE2;
    //WPUD=0X00;
 //   WPUE=0X01;        
    //
    //INLVLA=0X27;
    //INLVLB=0X00;
    //INLVLC=0X00;
    //INLVLD=0X00;      
    //
}

void timer0_Init(void)
{
    //T0CS=0;                             //T0 in timer Moudle
    //PSA=0;
    //PS0=0;
    //PS1=PS2=0;
    OPTION_REG=0b10000101;
    ///INTCON=0b10100000;
    T0IF=0;
    T0IE=1;      
    TMR0=T0_10ms;
    GIE=1;
}
void timer1_Init(void)
{
    T1CON=0X01; 
    //T1CKPS1=0;
    //T1CKPS0=1;
    
    TMR1IF=0;
    TMR1IE=1;      
    TMR1H=0x0B;
    TMR1L=0xDC;
    PEIE=1;
    GIE=1;
    TMR1IE=1;
    T1CONbits.TMR1ON=1;
}

 //void  interrupt  ISR(void)
void __interrupt(high_priority) ISR(void)
{
    //t0 interrupt service routine 改全局变量时间设定方式
    if(TMR0IF==1)
    {
    T0IF=0;
    TMR0=T0_10ms;
    if(WFLAG)
        {
        if((TimeFLAG==0)|(TimeFLAG==1 && countM!=0))
        {
            temph++;    
        }
        if(temph>60000)                 
            {
                temph=0;
                if(m<6)
                    {
                        m++;
                    }
                else
                {
                    m=0;
                    tempH++;  
                    tempHD=tempH;
                    eeprom_write(0x11,(tempH>>8)&0x00FF);  
                    eeprom_write(0x10,tempH&0x00FF); 
                    while(WR)
                    continue;     
                }
            }  
        }
    if(TimeFLAG)
        {
            temp1++;
            if(temp1>60000)
            {
                temp1=0;
            }
            tempM = temp1/6000;
            if(countM>=1&&tempM0!=tempM)
            {
                countM=countM-1;               
            }
            tempM0=tempM;
            //if(temp1>(countM*6000+1500))                //if(temp1>120),  FFFF=65535
            if(countM<1)
            {
            countM=0;
            temp1=0;
            PORTFbits.RF1=1;
            TimeFLAG=0;
            WFLAG=0;
            }
        }
    }
    if (TMR1IF==1) 
    {  
    TMR1H=0x0B;
    TMR1L=0xDC;
    TMR1IF=0;
    switch (Operation_Type) 
        {    
      case 1:  //V1?V2?????0.7s
            PORTFbits.RF3=1; 
            PORTFbits.RF4=1;
            if (++Time_Count != 6) return;
            Time_Count = 0;
            //PORTFbits.RF1=0;
            Operation_Type = 2; //????
            break;            
      case 2:  //
            PORTFbits.RF3=1; 
            PORTFbits.RF4=0;
            if (++Time_Count != 48) return;
            Time_Count  = 0; 
            Operation_Type = 3; 
            break;      
      case 3:  //
            PORTFbits.RF3=1; 
            PORTFbits.RF4=1;
            if (++Time_Count != 6) return;
            Time_Count = 0;
            //PORTFbits.RF1=1;
            Operation_Type = 4;     
            break;             //***//break??????????
      case 4:  //
            PORTFbits.RF3=0; 
            PORTFbits.RF4=1;  //5?????? (0.5s * 10 = 5
            if (++Time_Count != 48) return;
            Time_Count  = 0; 
            Operation_Type = 1; //????
            break;        
        }  
    }
    /*
        if(RC2IF==1)
    {
     RC2IF=0;
     receiveo2[receive_count]=RCREG;
     sendo2[receive_count]=RCREG;
                                 //************PORTD=RCREG;       //用led指示单片机接收到的数据。
     receive_count++;
     if(receive_count>11) 
     {
     receive_count=0;
     SciReceiveFlag=1;
     }  
    } 
    else if(TXIF==1)
    {

    } 
     
     */

    
}
void UART_SendByte(void)
{
     uchar i;
    SciReceiveFlag=0;
    for(i=0;i<3;i++)
    {
        TXREG=sendo2[i];
    while(1)
    {
     if(TXIF==1) break;   //等待数据发送完成，跳出死循环。
    }
    }
}
void main(void)
{
    Init();


    led_red=1;
    //TM1721_Init();
    CLK=1;DIO=1;
    STB=1; 
    TM1721_Write_Byte(0x08); //显示模式，字配置
    STB=1;
    TM1721_Write_Byte(0x97); //显示开OK
    STB=1;
    TM1721_DisplayClear(16);   //OK

    tempHD = eeprom_read(0x11)*256; 
    tempHD += eeprom_read(0x10); 
    if(tempHD!=0xFFFF)
    {    
    tempH=tempHD;
    dat[4] = SEG_CODE[(tempHD /10000) % 10];      //分解显示万位数
    dat[5] = SEG_CODE[(tempHD /1000) % 10];       //分解显示千位数
    dat[6] = SEG_CODE[(tempHD /100) % 10];      //分解显示百位数
    dat[7] = SEG_CODE[(tempHD /10) % 10];       //分解显示十位数
	dat[8] = SEG_CODE[tempHD % 10];             //分解显示个位数 
    }
    else
    {
    tempHD=tempH;              //tempHD=0x0000;
    dat[4] = SEG_CODE[0];      //分解显示万位数
    dat[5] = SEG_CODE[0];       //分解显示千位数
    dat[6] = SEG_CODE[0];      //分解显示百位数
    dat[7] = SEG_CODE[0];       //分解显示十位数
	dat[8] = SEG_CODE[0];             //分解显示个位数 
    }
    
    TM1721_Write_StringS(dat); 

    PORTBbits.RB1=0;           //fan   开始动作放循环 一次后退出 to do
    PORTBbits.RB2=0;           //Pump
    PORTFbits.RF3=1;
    PORTFbits.RF4=1;
    PORTFbits.RF1=1;           //LED I/O
    DELAY(160);
    PORTBbits.RB1=1;
    PORTBbits.RB2=1;           //PumpOn
    PORTFbits.RF3=0;
    PORTFbits.RF4=1;
    DELAY(40);
    PORTFbits.RF3=0;
    PORTFbits.RF4=0;
    DELAY(12);
    PORTFbits.RF3=0;
    PORTFbits.RF4=1;
    DELAY(192);
    PORTFbits.RF1=0;           //LED I/O  0=ON;
    timer0_Init();
    timer1_Init();  
    //UART TO measure 02%
    uchar i;
    SP2BRGL=25;             //波特率设置，fosc/16(x+1).
    TX2STA=0X24;           //8位数据，异步模式，高速。
    //TRISG=0xff;     			//G2(RX)设置为输入方式 G1=(TX)设置为输出方式
    BAUD2CONbits.BRG16=0;  //8位波特率发生器
    
    TRISG2=1;     			//(RX)
    TRISG1=1;     			//(TX)
    
    TX2STAbits.SYNC=0; 
    RC2STAbits.SPEN=1;
    
    TX2STAbits.TXEN=1;               //允许串行口发送数据
    
    //RC2STA=0X90;           //使能串行通信，8位接收。 
    PIR4=0;               //清中断标志
    PIE4=PIE4|0x30;        //允许接收中断。RC2IE,TX2IE=1
    //RC2STAbits.CREN=1;               //允许串行口连续接收数据
    

    TXREG=0x31;//发送寄存器发送0x31对应的’叫什么码的‘就是1这一步是指一上电或复位时会先发送一个数检验是否有发送及接收
    while(!TX2STAbits.TRMT);//判断TRMT是否自动置1,如果为1则说明发传送完毕，取反退出if循环
    
    while (1) 
    {
    //adcinit();   
    for(i=0;i<3;i++)
    {
        while(!TX2STAbits.TRMT);
            //{;}
        TXREG=sendo2[i];  
    }
     /*
      //if(TXIF==0)    //发送完8个数据。
        //{
        for(i=0;i<3;i++)
            {
                SciSendFlag=0;//
                //send_count=0;
                p=&sendo2[0];
                TXREG=*p; 
                p++;
                while(TX2STAbits.TRMT==0);    //TSR发送完成
                SciSendFlag=1;//
                delay(500);
            }
        //}  
      */   
  
     //if(TX2IF==1)
     //   {
     //    if(send_count>7) 
     //    {
      //    TX2STAbits.TXEN=1;            //使能串行口发送数据
      //    return;              //返回主程序
      //   }
      //  }
     if(RC2IF==1)
        {
         RC2IF=0;
         receiveo2[receive_count]=RCREG;
                                     //************PORTD=RCREG;       //用led指示单片机接收到的数据。
         receive_count++;
         if(receive_count>11) 
         {
         receive_count=0;
         SciReceiveFlag=1;
         }  
        } 
    /*
    //for (m = 0; m < 2; m++)        //从0x10地址写入D2，D2
  //{ eeprom_write(0x10 + m,m);  while(WR);  } 

    dat[4] = SEG_CODE[(tempHD /10000) % 10];      //分解显示万位数
    dat[5] = SEG_CODE[(tempHD /1000) % 10];       //分解显示千位数
    dat[6] = SEG_CODE[(tempHD /100) % 10];      //分解显示百位数
    dat[7] = SEG_CODE[(tempHD /10) % 10];       //分解显示十位数
	dat[8] = SEG_CODE[tempHD % 10];             //分解显示个位数 
        
     STB=0;
     TM1721_Write_Byte(0x42);//送读按键命令（42H） 
     TRISD=0X20;   
     for(int e=0;e<3;e++) 
         { Key_arr[e]=TM1721_Read_KeyCC(); //读8位按键数据     
         __delay_ms(10);
         }   
     
    //UART TO measure 02% 
    //if(RCREG==0x11)
    //   RE1=1;                    //如果接收到AA命令继电器吸合
    //   else if(RCREG==0X55)
    //   RE1=0;                    //接收到55命令继电器断开
    
    //printf("Key_arr[0] : %s\n\n", Key_arr[0]); 
    
     //if(Key_arr[0]==KeyValue && Key_arr[0]!=0x00)
     //{
         
    switch(Key_arr[0])
    {
        //TimeFLAG= 1 ;
        case 0x80:                //s1   KS1与KEY3按键按下，Runtime+
            TimeFLAG= 1 ;
            WFLAG=1;
        if (countM<120)
        {countM+=1; }
        else 
            countM=0;
        break;
        case 0x08:                //s2    KS1与KEY2按键按下，Runtime-
            TimeFLAG= 1 ;
            WFLAG=1;
        if (countM>0)
        {countM-=1; }
        else 
            countM=0;
        break;
        case 0x40:                //s3    KS0与KEY3按键按下，ON/OFF
        //WFLAG=!WFLAG;                      //
            if(WFLAG==1)
            {
                WFLAG=0;
            }
            if(WFLAG==2)
            {
                WFLAG=1;
            }
            TimeFLAG= 0 ;
            PORTFbits.RF1=!PORTFbits.RF1;
                eeprom_write(0x11,(tempH>>8)&0x00FF);  
                eeprom_write(0x10,tempH&0x00FF);  
                while(WR)
                continue;    // read from EEPROM at address
        break;
        //default:
        //TimeFLAG= 1 ;
        //break; 
    }
    //}
    KeyValue=Key_arr[0]; 
    
    dat[1] = SEG_CODE[(countM /100) % 10];      //分解显示百位数
    dat[2] = SEG_CODE[(countM /10) % 10];       //分解显示十位数
	dat[3] = SEG_CODE[countM % 10];             //分解显示个位数 
    TRISD=0X00; 
    TM1721_Write_StringS(dat); 
    
       switch (WFLAG) 
        {    
            case 1:                     //Work    
                GIE=1;
                
                //timer0_Init();
                if(TimeFLAG==1&&WFLAG==0)
                {break;} 
                else
                {
                    PORTBbits.RB1=0;           //fan   开始动作放循环 一次后退出 to do
                    PORTBbits.RB2=1;           //PumpOn
                 TMR1IE=1;
                 timer1_Init();
                 
                }             
            break;            
            case 0:                     //Oxygen Concentrator Runtime achieved
                GIE=0;
                TMR1IE=0;
                DELAY(30);
                PORTBbits.RB2=0;
                PORTFbits.RF3=1;
                PORTFbits.RF4=1;
                //PORTFbits.RF1=0;
                DELAY(200);
                PORTFbits.RF3=0;
                PORTFbits.RF4=0;
                PORTFbits.RF1=1;
                WFLAG=2;
                break;  
            case 2:                      //待机ready 
                //return;       //Return to main()
                break;          //JUMP out of this switch 
        }
    */   
    }
}
/**
 End of File
 *   
*/




