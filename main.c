#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include "ws2812.c"
#include "board.c"

int main(void)
{
	led_clear();
	led_pixel(0, 0, 255, 0, 0);
	led_update();

    // Generate Board with mines
	generateField(5);
	revealAllFields();

	// show initial field
	led_clear();
	displayField();
	led_update();

	for (;;) {
		// TODO: Check for Cursor input
		// TODO: Handle cursor action (dig/flag)
		// TODO: Update Gamestate
		// TODO: Render Game
	}

	return 0;
}
