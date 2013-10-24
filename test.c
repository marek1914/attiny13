#define F_CPU 4.8E6
#include <avr/io.h>
#include <util/delay.h>

void Init_IO(void);
void display_led (char num);

unsigned char led = 0x0f;

int main(void) 
{
    Init_IO();
    while (1)
    {
	display_led (4);
    }
}

void Init_IO (void)
{
   DDRB = 0xff;
}

void display_led (char num)
{
    char i;
    
    for (i = 0; i <= num; i++)
    {
	PORTB = (led >> i); 
	_delay_ms (50); 
    } 
    led = 0x0f;
}
