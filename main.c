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
//int *p;                                                     //�ٹ̶���ַģʽ��ʾ0 1 2 3 4
//int dat[]={0xAF,0xA0,0x6D,0xE9,0xE2,0xCB,0xCF,0xA1,0xEF};   //��
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
 TXSTA=0X04;           //8λ���ݣ��첽ģʽ�����١�
 RCSTA=0X80;           //ʹ�ܴ���ͨ�ţ�8λ���ա�
 TRISG2=1;     			//(RX)����Ϊ���뷽ʽ
 TRISG1=0;     			//(TX)����Ϊ�����ʽ
 SPBRG=25;             //���������ã�fosc/16(x+1).
 PIR1=0;               //���жϱ�־
 PIE1=PIE1|0x20;        //��������жϡ�RCIE,TXIE
 PIE1=PIE1|0X10;
 RC2STAbits.CREN=1;               //�����п�������������
 TX2STAbits.TXEN=1;               //�����пڷ�������
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

// ====== ??(n �1�7 ) ? s+12 ? s ,??,?????????
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
    ADCON0=0x0C;             //ѡ��AN3ͨ������ʹ��ADC
    ADCON1=0x10;		      //ѡ������뷽ʽ,ADCʱ����ΪFOSC/8,
                              //VREF-��VSS���ӣ�VREF+��VDD���ӣ�ѡ��VDD��Ϊ�ο���ѹ
                              //������������ѹ��Ϊ�ο���ѹ�޸�ADCON1�Ĵ�����3λ����
    

    
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
    //t0 interrupt service routine ��ȫ�ֱ���ʱ���趨��ʽ
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
                                 //************PORTD=RCREG;       //��ledָʾ��Ƭ�����յ������ݡ�
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
     if(TXIF==1) break;   //�ȴ����ݷ�����ɣ�������ѭ����
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
    TM1721_Write_Byte(0x08); //��ʾģʽ��������
    STB=1;
    TM1721_Write_Byte(0x97); //��ʾ��OK
    STB=1;
    TM1721_DisplayClear(16);   //OK

    tempHD = eeprom_read(0x11)*256; 
    tempHD += eeprom_read(0x10); 
    if(tempHD!=0xFFFF)
    {    
    tempH=tempHD;
    dat[4] = SEG_CODE[(tempHD /10000) % 10];      //�ֽ���ʾ��λ��
    dat[5] = SEG_CODE[(tempHD /1000) % 10];       //�ֽ���ʾǧλ��
    dat[6] = SEG_CODE[(tempHD /100) % 10];      //�ֽ���ʾ��λ��
    dat[7] = SEG_CODE[(tempHD /10) % 10];       //�ֽ���ʾʮλ��
	dat[8] = SEG_CODE[tempHD % 10];             //�ֽ���ʾ��λ�� 
    }
    else
    {
    tempHD=tempH;              //tempHD=0x0000;
    dat[4] = SEG_CODE[0];      //�ֽ���ʾ��λ��
    dat[5] = SEG_CODE[0];       //�ֽ���ʾǧλ��
    dat[6] = SEG_CODE[0];      //�ֽ���ʾ��λ��
    dat[7] = SEG_CODE[0];       //�ֽ���ʾʮλ��
	dat[8] = SEG_CODE[0];             //�ֽ���ʾ��λ�� 
    }
    
    TM1721_Write_StringS(dat); 

    PORTBbits.RB1=0;           //fan   ��ʼ������ѭ�� һ�κ��˳� to do
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
    SP2BRGL=25;             //���������ã�fosc/16(x+1).
    TX2STA=0X24;           //8λ���ݣ��첽ģʽ�����١�
    //TRISG=0xff;     			//G2(RX)����Ϊ���뷽ʽ G1=(TX)����Ϊ�����ʽ
    BAUD2CONbits.BRG16=0;  //8λ�����ʷ�����
    
    TRISG2=1;     			//(RX)
    TRISG1=1;     			//(TX)
    
    TX2STAbits.SYNC=0; 
    RC2STAbits.SPEN=1;
    
    TX2STAbits.TXEN=1;               //�����пڷ�������
    
    //RC2STA=0X90;           //ʹ�ܴ���ͨ�ţ�8λ���ա� 
    PIR4=0;               //���жϱ�־
    PIE4=PIE4|0x30;        //��������жϡ�RC2IE,TX2IE=1
    //RC2STAbits.CREN=1;               //�����п�������������
    

    TXREG=0x31;//���ͼĴ�������0x31��Ӧ�ġ���ʲô��ġ�����1��һ����ָһ�ϵ��λʱ���ȷ���һ���������Ƿ��з��ͼ�����
    while(!TX2STAbits.TRMT);//�ж�TRMT�Ƿ��Զ���1,���Ϊ1��˵����������ϣ�ȡ���˳�ifѭ��
    
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
      //if(TXIF==0)    //������8�����ݡ�
        //{
        for(i=0;i<3;i++)
            {
                SciSendFlag=0;//
                //send_count=0;
                p=&sendo2[0];
                TXREG=*p; 
                p++;
                while(TX2STAbits.TRMT==0);    //TSR�������
                SciSendFlag=1;//
                delay(500);
            }
        //}  
      */   
  
     //if(TX2IF==1)
     //   {
     //    if(send_count>7) 
     //    {
      //    TX2STAbits.TXEN=1;            //ʹ�ܴ��пڷ�������
      //    return;              //����������
      //   }
      //  }
     if(RC2IF==1)
        {
         RC2IF=0;
         receiveo2[receive_count]=RCREG;
                                     //************PORTD=RCREG;       //��ledָʾ��Ƭ�����յ������ݡ�
         receive_count++;
         if(receive_count>11) 
         {
         receive_count=0;
         SciReceiveFlag=1;
         }  
        } 
    /*
    //for (m = 0; m < 2; m++)        //��0x10��ַд��D2��D2
  //{ eeprom_write(0x10 + m,m);  while(WR);  } 

    dat[4] = SEG_CODE[(tempHD /10000) % 10];      //�ֽ���ʾ��λ��
    dat[5] = SEG_CODE[(tempHD /1000) % 10];       //�ֽ���ʾǧλ��
    dat[6] = SEG_CODE[(tempHD /100) % 10];      //�ֽ���ʾ��λ��
    dat[7] = SEG_CODE[(tempHD /10) % 10];       //�ֽ���ʾʮλ��
	dat[8] = SEG_CODE[tempHD % 10];             //�ֽ���ʾ��λ�� 
        
     STB=0;
     TM1721_Write_Byte(0x42);//�Ͷ��������42H�� 
     TRISD=0X20;   
     for(int e=0;e<3;e++) 
         { Key_arr[e]=TM1721_Read_KeyCC(); //��8λ��������     
         __delay_ms(10);
         }   
     
    //UART TO measure 02% 
    //if(RCREG==0x11)
    //   RE1=1;                    //������յ�AA����̵�������
    //   else if(RCREG==0X55)
    //   RE1=0;                    //���յ�55����̵����Ͽ�
    
    //printf("Key_arr[0] : %s\n\n", Key_arr[0]); 
    
     //if(Key_arr[0]==KeyValue && Key_arr[0]!=0x00)
     //{
         
    switch(Key_arr[0])
    {
        //TimeFLAG= 1 ;
        case 0x80:                //s1   KS1��KEY3�������£�Runtime+
            TimeFLAG= 1 ;
            WFLAG=1;
        if (countM<120)
        {countM+=1; }
        else 
            countM=0;
        break;
        case 0x08:                //s2    KS1��KEY2�������£�Runtime-
            TimeFLAG= 1 ;
            WFLAG=1;
        if (countM>0)
        {countM-=1; }
        else 
            countM=0;
        break;
        case 0x40:                //s3    KS0��KEY3�������£�ON/OFF
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
    
    dat[1] = SEG_CODE[(countM /100) % 10];      //�ֽ���ʾ��λ��
    dat[2] = SEG_CODE[(countM /10) % 10];       //�ֽ���ʾʮλ��
	dat[3] = SEG_CODE[countM % 10];             //�ֽ���ʾ��λ�� 
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
                    PORTBbits.RB1=0;           //fan   ��ʼ������ѭ�� һ�κ��˳� to do
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
            case 2:                      //����ready 
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




