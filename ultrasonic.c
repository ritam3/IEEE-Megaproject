/*
 * GccApplication1.c
 *
 * Created: 25-06-2019 21:23:47
 * Author : RITAM UPADHYAY
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define SETBIT(ADDRESS, BIT)    (ADDRESS |= (1 << BIT))
#define CLEARBIT(ADDRESS, BIT)  (ADDRESS &= ~(1 << BIT))
#define CHECKBIT(ADDRESS, BIT)  (ADDRESS & (1 << BIT))
#define SW2 PINE1
int r=0,time=0;int range =0;
void inipins()
{
	DDRB|= (1<<PB0);
	DDRD|= 0x00;
	PORTB|= 0x00;
}
ISR(INT0_vect)
{
	if(r==1)
	{
		TCNT1 = 0;
		TCCR1B = (1<<CS02)|(1<<CS00);
		falling();
		r= 0;
	}
	else
	{
		CLEARBIT(EIMSK,INT0);
		TCCR1B = 0x00;
		time = TCNT1L;
		time|= (TCNT1H<<8);
		range= time*64/58;
		PORTB&= ~(1<<PB0);
	}
}
void rise()
{
	r= 1;
	EIMSK|= (1<<INT0);
	EICRA =0x03;
}
void trig()
{
	PORTB|=(1<<PB0);
	_delay_us(10*16);
	CLEARBIT(PORTB,PB0);
	rise();
}
void falling()
{
	EIMSK|= (1<<INT0);
	EICRA =0x02;
}
void servinit()
{
	TCCR3A|=(1<<WGM31)|(1<<COM3A1);
	TCCR3B|=(1<<WGM33)|(1<<WGM32)|(1<<CS31);
	DDRC|=(1<<PC6)|(1<<PC5)|(1<<PC4);
	ICR3 = 40000;
}
unsigned int servoang(int d)
{
	unsigned int res;
	res= (unsigned int)(20.62*d)+1088;
	return res;
}
void USART_Init( unsigned int ubrr )
{
	/* Set baud rate */
	UBRR1H = (unsigned char)(ubrr>>8);
	UBRR1L = (unsigned char)ubrr;
	
	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	
	/* Set frame format: 8data, 1stop bit */
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);
}
void USART_Transmit( unsigned char ar )
{
	
	while ( !( UCSR1A & (1<<UDRE1)) )
	{
		
	}
		 UDR1 = ar;
}
unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR1A & (1<<RXC1)) )
	{
		
	}
	/* Get and return received data from buffer */
	return UDR1;
}
void sendstring(unsigned char c[])
{
	for(int i=0;c[i]!='\0';i++)
	{
		USART_Transmit(c[i]);
	}
}
double a[181][2];
int main(void)
{
	char c[100];
	inipins();
	DDRE|=(1<<DDE6);
	sei(); 
	servinit();
	unsigned char data[5];
	USART_Init(8);
	int i;
    while (1) 
    {
		
		for(i=180;i>=0;i--)
		{
			OCR3A = servoang(i);
			_delay_ms(50*16) ;
			trig();
			_delay_ms(10*16);
			 sprintf(data,"%04d",i);
			 sendstring(data);
			 USART_Transmit(' ');
			 sprintf(data,"%04d",range);
			 sendstring(data);
			 USART_Transmit(13);		 
		}
		for(i=0;i<=180;i++)
		{
			OCR3A = servoang(i);
			_delay_ms(50*16) ;
			trig();
			_delay_ms(10*16);
			 sprintf(data,"%04d",i);
			 sendstring(data);
			 USART_Transmit(' ');
			 sprintf(data,"%04d",range);
			 sendstring(data);
			  USART_Transmit(13);
		}
    }
	return 0;
}

