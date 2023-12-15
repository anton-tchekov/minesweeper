#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include "ws2812.c"

int main(void)
{
	led_clear();
	led_pixel(0, 0, 255, 0, 0);
	led_update();

	for(;;)
	{
	}

	return 0;
}
