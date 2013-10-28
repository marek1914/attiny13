#define F_CPU 4.8E6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned int adc_data;
volatile unsigned char time_ok;
volatile unsigned char light = 0;
volatile char mark = 1;
unsigned char led = 0x0f;
unsigned int adc_value;

void Init_IO(void);
void display_led_mode1 (char num); 
void display_led_mode2 (void);
void Init_timer_led ();
void Init_timer_adc ();
void Init_ADC ();

int main(void) 
{
    Init_IO();
   // display_led_mode2 ();
    while (1)
    {
	if (time_ok)
	{
	    time_ok = 0; 
	   // adc_value = (unsigned long) adc_data * 1100 / 1024;
    	}
	if (adc_data == 0)
	    display_led_mode1 (2);
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
    
    for (i = 0; i <= num; i++)
    {
	PORTB = (led >> i); 
	_delay_ms (50); 
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

//ADC模式定时器初始化
void Init_timer_adc ()
{
    TCCR0A = (1<<WGM01)|(1<<WGM00);
    TCCR0B = (1<<CS01)|(1<<CS00);
    TCNT0 = 0X00;
    OCR0B = 0X96; //控制取样频率
    TIMSK0 = (1<<OCF0B)|(1<<TOIE0);
}

ISR (TIM0_COMPA_vect)
{
    PORTB = 0x00;
}

ISR (TIM0_COMPB_vect)
{
    time_ok = 1;
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

//ADc转换结束中断
ISR (ADC_vect)
{
    adc_data = ADCW;
}

void Init_ADC ()
{
    Init_timer_adc ();
    ADMUX = (1<<REFS0)|(1<<MUX1);
    ADCSRA = (1<<ADEN)|(1<<ADATE)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1);
    ADCSRB = (1<<ADTS2)|(1<<ADTS0); //触发源选用T/C比较匹配B
    DIDR0 = (1<<ADC2D);
    sei();
}
