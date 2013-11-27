#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char times, s_ok;
int main (void)
{
	s_ok = 0;
	TCCR0A = (1<<WGM01); //CTC
	TCCR0B = (1<<CS02); //256分频 16M/256=62.5khz
	0CR0A = 0x7c; //2ms 触发一次
	TIMSK0 = |(1<<OCIE0A);
}

ISR (TIM0_OVF_vect)
{
     times++
     if (times >= 500)
     {
	times = 0;
	s_ok = 1;//1s到了
     }
}


