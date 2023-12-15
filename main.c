#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include "ws2812.c"

int main(void)
{
	led_clear();
	led_pixel(0, 0, 255, 0, 0);
	led_update();

	// TODO: Define Board Structure
	// TODO: Generate Board with mines

	for(;;)
	{
		// TODO: Check for Cursor input
		// TODO: Handle cursor action (dig/flag)
		// TODO: Update Gamestate
		// TODO: Render Game
	}

	return 0;
}

