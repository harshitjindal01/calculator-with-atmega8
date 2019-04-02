/*******************************************************
Project : To design a calulator using a microcontroller, lcd and keypad.
Version : 
Date    : 02-02-2019
Author  : harshit jindal
Company : JMIT,RADAUR
Comments:
************************ Header files and Macros *******************************/

#include <io.h>
#include <delay.h>
#include <mega8.h>
#include <math.h>
#define F_XTAL 12000000L

#define E PORTC.0           //enable pin of LCD
#define RS PORTC.1          //Register Select pin of LCD

#define RowA PORTD.0        //to send the data to keypad
#define RowB PORTD.1 
#define RowC PORTD.2 
#define RowD PORTD.3 

#define Col1 PIND.4        //to receive the data from keypad
#define Col2 PIND.5 
#define Col3 PIND.6 
#define Col4 PIND.7 
/**************************** Global variables *****************************/


/**************************** LCD functions *****************************/

void lcd_cmd(char a)
{
    E=1;
    RS=0;  
    PORTB=a; 
    delay_ms(5);
    E=0;
    delay_ms(5);
} 
    
void lcd_data(char b)
{ 
    E=1; 
    RS=1;
    PORTB =b;
    delay_ms(5);
    E=0;
    delay_ms(5);
} 
    
void print_string(char *str)
{
     while(*str!='\0')
     {               
      lcd_data(*str);
      str++;
     }
}

void init_LCD()
{
   
  lcd_cmd(0x38);        //LCD initialization
  lcd_cmd(0x0c);        //display on cursor off
  lcd_cmd(0x01);        //clear display screen
  lcd_cmd(0x06);        //shift 
  lcd_cmd(0x02);        //return to home
}

void port_init()
{
    DDRB=0xFF;    //LCD
    DDRC=0XFF;    //LCD
    DDRD=0x0F;    //Keypad ( upper nibble-in, lower nibble-out )
}

/**************************** Keypad functions *****************************/

char READ_SWITCHES(void)    
{    
    RowA = 1; RowB = 0; RowC = 0; RowD = 0;     //Test Row A
    if (Col1 == 1) { delay_ms(10); while (Col1==1); return '7'; }
    if (Col2 == 1) { delay_ms(10); while (Col2==1); return '8'; }
    if (Col3 == 1) { delay_ms(10); while (Col3==1); return '9'; }
    if (Col4 == 1) { delay_ms(10); while (Col4==1); return '/'; }

    RowA = 0; RowB = 1; RowC = 0; RowD = 0;     //Test Row B
    if (Col1==1) { delay_ms(10); while (Col1==1); return '4'; }
    if (Col2==1) { delay_ms(10); while (Col2==1); return '5'; }
    if (Col3==1) { delay_ms(10); while (Col3==1); return '6'; }
    if (Col4==1) { delay_ms(10); while (Col4==1); return '*'; }
    
    RowA = 0; RowB = 0; RowC = 1; RowD = 0;     //Test Row C
    if (Col1==1) { delay_ms(10); while (Col1==1); return '1'; }
    if (Col2==1) { delay_ms(10); while (Col2==1); return '2'; }
    if (Col3==1) { delay_ms(10); while (Col3==1); return '3'; }
    if (Col4==1) { delay_ms(10); while (Col4==1); return '-'; }
    
    RowA = 0; RowB = 0; RowC = 0; RowD = 1;     //Test Row D
    if (Col1==1) { delay_ms(10); while (Col1==1); return 'C'; }
    if (Col2==1) { delay_ms(10); while (Col2==1); return '0'; }
    if (Col3==1) { delay_ms(10); while (Col3==1); return '='; }
    if (Col4==1) { delay_ms(10); while (Col4==1); return '+'; }

    return 'n';               // Means no key has been pressed
}


char get_key(void)           //get key from user
{
    char key = 'n';              //assume no key pressed

    while(key=='n')              //wait untill a key is pressed
    {       
      key = READ_SWITCHES();   //scan the keys again and again
    }          
        lcd_data(key);
        delay_ms(20); 
    return key;                  //when key pressed then return its value
}

/**************************** Calculator related functions**************************/

char IsOperator(char ch)
{
  if(ch == '*' || ch=='+' || ch=='/' || ch=='-')
    return 1;
  else 
    return '\0';  
}

char IsDigit(char ch)
{
  if(ch>=48 && ch<=57)
   return 1;
  else
   return '\0';
}

char IsClear(char ch)
{
  if(ch=='C')
   return 1;
  else
   return '\0';
}

void print_result(int res)
{
  char pos = 0xCF;
  lcd_cmd(pos);
   if(res==0)
   {
     lcd_data('0');
   }
   
   while(res!=0)
   {
     lcd_data(res%10 + '0');
     pos = pos - 1;
     res = res/10;
     lcd_cmd(pos);
   } 
}

void Calculate(int op1, int op2, char Operation)
{
   int res=0;
   if(Operation == '+')
    res = op1+op2;
   else if(Operation == '-')
    res = op1-op2;
   else if(Operation == '*')
    res = op1*op2;
   else if(Operation == '/')
    res = op1/op2;
   else 
   res=op1;  
   print_result(res);
}

/**************************** Main and other functions *****************************/

void device_init()
{
    port_init();
    init_LCD();
} 

void print_welcome_msg()
{
    lcd_cmd(0x83);
    print_string("WELCOME TO");
    lcd_cmd(0xC5);
    print_string("EMR CLUB");
    delay_ms(20); 
}


void main()
{
    char flag_op = 0;
    int op1=0, op2=0;
    char Operator = 'n';

    device_init();
   // print_welcome_msg();
    lcd_cmd(0x01); 
    lcd_cmd(0x80);
  while(1)
  {
     char ch = get_key();
     if(IsDigit(ch))
    {   
        if(flag_op == 0)
        {
          op1 = 10*op1 + (ch - '0');
        }
        else 
        {
          op2 = 10*op2 + (ch - '0');
        }
    }
    else if(IsOperator(ch))
    {
       if(flag_op==1){ lcd_cmd(0x01); lcd_cmd(0x80); print_string("Invalid Input");continue;}
       
       Operator = ch; 
       flag_op = 1;        
    }
    else if(IsClear(ch))
    {
      flag_op = 0;
      lcd_cmd(0x01);
      lcd_cmd(0x80);
      Operator = 'n';
    }
    else if(ch=='=')
    {
      Calculate(op1, op2, Operator);
	  op1=0;
	  op2=0;
	  Operator='n';
    }
 }
}

