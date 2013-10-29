#define F_CPU 4.8E6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned char light = 0;
volatile char mark = 1;
unsigned char led = 0x0f;
unsigned int adc_value;

void Init_IO(void);
void display_led_mode1 (char num); 
void display_led_mode2 (void);
void Init_timer_led ();
void Init_ADC ();
int ReadADC (void);

int main(void) 
{
    Init_IO ();
    Init_ADC ();
   // display_led_mode2 ();
    while (1)
    {
	ReadADC ();
	adc_value = ReadADC ();
	display_led_mode1 (adc_value / 12);
    }
}

void Init_IO (void)
{
   DDRB = 0x0f;
   PORTB = 0X0F;
}

void display_led_mode1 (char num)
{
    char i;
    if (num > 4)
	num = 4;
    
    for (i = 0; i <= num; i++)
    {
	PORTB = (led >> i); 
	_delay_ms (30); 
    } 
    led = 0x0f;
}
   
void display_led_mode2 (void)
{
    Init_timer_led();   
}

//led模式寄存器初始化
void Init_timer_led ()
{
    
    TCCR0A = (1<<WGM01)|(1<<WGM00); 
    TCCR0B = (1<<CS01)|(1<<CS00);
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
    if (mark == 1)
        OCR0A = light++;
    else
        OCR0A = light--;
    if ((light == 0xff) || (light == 0x00))
	mark = ~mark;
}

void Init_ADC (void)
{
   ADMUX |= (1 << REFS0); // 1.1v as Reference 
   ADMUX |= (1 << MUX1) | (0 << MUX0); // ADC2 PB.4 
   ADCSRA |= (1 << ADEN); // Enable ADC
}

int ReadADC(void)
{
      ADMUX |= (1 << REFS0); // VCC as Reference 
      ADMUX |= 2;// ADC2 PB.4 
      ADCSRA |= (1 << ADSC); // Start Converstion 
      while((ADCSRA & 0x40) !=0){}; //wait for conv complete 
      return ADCW;
}
                
