#define F_CPU 4.8E6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned char light = 254;
volatile char mark = 1;
unsigned char led = 0x0f;

void Init_IO(void);
void display_led_mode1 (char num); 
void display_led_mode2 (void);
void Init_timer ();

int main(void) 
{
    Init_IO();
    display_led_mode2 ();
    while (1)
    {
	//display_led_mode1 (4);
    }
}

void Init_IO (void)
{
   DDRB = 0xff;
}

void display_led_mode1 (char num)
{
    char i;
    
    for (i = 0; i <= num; i++)
    {
	PORTB = (led >> i); 
	_delay_ms (50); 
    } 
    led = 0x0f;
}
   
void display_led_mode2 (void)
{
    Init_timer();   
}

void Init_timer ()
{
    TCCR0A = 0X03;
    TCCR0B = 0X03;
    TCNT0 = 0X00;
    OCR0A = 0x00;
    TIMSK0 |= (1<<OCF0A)|(1<<TOIE0);
    sei();
}

ISR (TIM0_COMPA_vect)
{
    PORTB = 0x00;
}

ISR (TIM0_OVF_vect)
{
    PORTB = 0Xff;
    if ((light == 0xff) | (light == 0x00))
	mark =~ mark;
    if (mark == 1)
        OCR0A = light++;
    else
        OCR0A = light--;

}

