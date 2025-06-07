
#include"4bitcode.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<time.h>
#include"motor.h"
#include"uart0.h"
#include"i2c_eeprom.h"
#include "keypad.h"

#define sw 14

unsigned char byte,byte1,arr1[20],arr2[5];
int i=0,j,digit,c=0;
int otp=1111;
int match = 1;
unsigned char string[10];
char mssg[50];

int main()
{
	lcd_confi();
	UART0_config();
	motot_init();

	lcd_string("Multilevel sercutity based access control system");
	//lcd_scroll("Multilevel sercutity based access control system");
	delay_ms(500);
	lcd_com(0x01);
	init_i2c();
	i2c_eeprom_page_write(0x50,0x00,"1234",4);
	i2c_eeprom_seq_read(0x50,0x00,p,4);
	delay_ms(150);
//	p[4]='\0';//don't want
//	lcd_string(p);
	while(1)
	{																 
		while(((IOPIN0>>sw)&1)==1);
		delay_ms(200);//switch debouncing
		otp++;
		lcd_com(0x01);
		lcd_com(0x80);
		lcd_string("ENTER PASSWORD");
		lcd_com(0xc0); 
		for(i=0;i<=3;i++)
		{
 			byte=keyscan()+48;
  			lcd_data('*');
  			arr2[i]=byte;
		}
		arr2[i]='\0';
	   lcd_string(arr2);
	   delay_ms(2000);
	if ((strncmp(p,arr2,4))==0)
   {
 		lcd_com(0x01);
		lcd_com(0x80);
  	 	lcd_string("PASSWORD MATCHED");
		delay_ms(500);
		
		lcd_com(0x01);
		lcd_com(0x80);
		lcd_string("Generating OTP");
		delay_ms(500);
   		
		lcd_com(0x01);
		lcd_com(0x80);
  	 	lcd_string("OTP SENDING.......");
  
        UART0_string("AT\r\n");
	    delay_ms(1000);
	    UART0_string("AT+CMGF=1\r\n");
	    delay_ms(1000);
	    UART0_string("AT+CMGS=\"+919344825266\"\r\n");
	    delay_ms(1000);

		sprintf(mssg,"%d",otp);	//mssg : your OTP is 1234
		UART0_string("your otp is:");
		UART0_string(mssg);
		UART0_string("\r\n");
	//	UART0_string("string\r\n");
		UART0_TX(0x1A);  // CTRL+Z to send SMS
        delay_ms(2000);
		   
		lcd_com(0x01);
 		lcd_com(0x80);
 	    lcd_string("Enter OTP");
	    lcd_com(0xc0);
	
		for(i=0;i<4;i++)
		  {
		  byte1=keyscan()+48;
		  arr1[i]=byte1;
		  }
		  arr1[4]='\0';
		 
	    if(strcmp(mssg,arr1)==0)	 //comparing otp and entered otp is matching or not
		 {
		   lcd_com(0x01);
 		   lcd_com(0x80);
		   lcd_string("otp matched");
		   delay_ms(1000);
		   motor_clock();
		   lcd_com(0x01);
		   lcd_string("Door open");
		   delay_motor(5);
		   motor_anticlock();
		  lcd_string("Door closed");
		  IOCLR0 |=motor_pin1|motor_pin2;
		 } 
		 else 
		 {
		 lcd_string("otp is wrong");
		 c++;
		 if(c==3)
		 {
		 UART0_string("AT+CMGS=\"+919160945102\"\r\n");
		 delay_ms(1000);
		 UART0_string("someone accessing your door\r\n");
		 }
		 }
		 
		 } 

  else
  {
  lcd_com(0x01);
   lcd_string("Password Wrong");
  }
delay_ms(1000);  // Delay to show message
lcd_com(0x01);   // Clear screen
}
}
