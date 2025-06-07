//#include<LPC21xx.h>
//#include"4bitcode.h"
#define c0 (IOPIN0&(1<<16))
#define c1 (IOPIN0&(1<<17))
#define c2 (IOPIN0&(1<<18))
#define c3 (IOPIN0&(1<<19))
#define r0 1<<20
#define r1 1<<21
#define r2 1<<22
#define r3 1<<23
typedef unsigned char u8;
unsigned int key_lut[4][4]={{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
unsigned int keyscan(void)
{
u8 row_val,col_val;
//PINSEL0|=0x00000000;
IODIR0|=r0|r1|r2|r3;
while(1)
{
IOCLR0|=r0|r1|r2|r3;
IOSET0|=c0|c1|c2|c3;
while((c0&&c1&&c2&&c3)==1);
IOCLR0|=r0;
IOSET0|=r1|r2|r3;
if((c0&&c1&&c2&&c3)==0)
{
row_val=0;
break;
}

IOCLR0|=r1;
IOSET0|=r0|r2|r3;
if((c0&&c1&&c2&&c3)==0)
{
row_val=1;
break;
}
IOCLR0|=r2;
IOSET0|=r0|r1|r3;
if((c0&&c1&&c2&&c3)==0)
{
row_val=2;
break;
}
IOCLR0|=r3;
IOSET0|=r0|r1|r2;
if((c0&&c1&&c2&&c3)==0)
{
row_val=3;
break;
}
}
if(c0==0)
col_val=0;
else if(c1==0)
col_val=1;
else if(c2==0)
col_val=2;
else 
col_val=3;
delay_ms(250);
while((c0&&c1&&c2&&c3)==0);
return key_lut[row_val][col_val];
}
/*int main()
{
int res;
lcd_confi();
lcd_string("4x4 matrix keypad");
while(1)
{
lcd_com(0xc0);
res=keyscan();
lcd_integer(res);
delay_ms(500);
 lcd_com(0x01);
}
} */


							    