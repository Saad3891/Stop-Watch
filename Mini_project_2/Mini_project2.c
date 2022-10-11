#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//global variables for count
unsigned char seconds=0 ;
unsigned char minutes=0;
unsigned char hours=0;

void Seven_Segment_init(void);
void Timer1_init(void);
void stop_watch_Reset (void);
void stop_watch_Pause(void);
void stop_watch_Resume (void);


int main ()
{
	Seven_Segment_init();
	stop_watch_Reset (); //function for reset button
	stop_watch_Pause();  //function for pause button
	stop_watch_Resume ();  // function for resume button
	Timer1_init();  //function for timer1 with compare mode

	while (1)
	{

		PORTA = (1<<PA0);
		PORTC = seconds%10;
		_delay_ms(2);

		PORTA = (1<<PA1);
		PORTC = seconds/10;
		_delay_ms(2);

		PORTA = (1<<PA2);
		PORTC = minutes%10;
		_delay_ms(2);

		PORTA = (1<<PA3);
		PORTC = minutes/10;
		_delay_ms(2);

		PORTA = (1<<PA4);
		PORTC = hours%10;
		_delay_ms(2);

		PORTA = (1<<PA5);
		PORTC = hours/10;
		_delay_ms(2);
	}
}

//ALL THE ISR FOR ALL INTERRPUT
//timer_interrput
ISR (TIMER1_COMPA_vect)
{
	seconds++;

	if (seconds == 60)
	{
		minutes++;
		seconds=0;
	}

	if (minutes == 60)
	{
		hours++;
		minutes=0;
	}
}

//for reset
ISR (INT0_vect)  {
	hours = 0;
	minutes = 0;
	seconds = 0;
}
// PAUSE BUTTOM
ISR (INT1_vect)
{
	TCCR1B &= ~(1<<CS10);
	TCCR1B &= ~(1<<CS11);
	TCCR1B &= ~(1<<CS12);
}
// for resume
ISR (INT2_vect)
{
	TCCR1B |= (1<<CS10);
	TCCR1B |= (1<<CS12);
}



//7 segment function
void Seven_Segment_init(void)
{
	DDRC |= 0x0F;   //INITIALIZE FIRST 4-PINS IN PORTC
	DDRA |= 0x3F;   //INITIALIZE FIRST 6-PINS IN PORTA
	PORTC =0;       //TURN OFF PORTC
}

void Timer1_init(void)
{
	SREG |= (1<<7);                                 //global interrupt enable
	TCCR1B=(1<<CS12)|(1<<CS10)|(1<<WGM12);
	TCNT1 = 0;
	OCR1A = 977;
	TIMSK |= (1<<OCIE1A);
}

void stop_watch_Reset (void)
{
	DDRD &= ~(1<<PD2);
	PORTD |= (1<<PD2);
	GICR |= (1<<INT0);
	MCUCR |= (1<<ISC01);
}

void stop_watch_Pause(void)
{
	GICR |= (1<<INT1);  //enable INT1
	MCUCR |= (1<<ISC10) | (1<<ISC11);
}

void stop_watch_Resume (void)
{
	DDRB &= ~(1<<PB2);
	PORTB |= (1<<PB2);
	GICR |= (1<<INT2);        //enable INT2
	MCUCSR &= ~(1<<ISC2);
}




