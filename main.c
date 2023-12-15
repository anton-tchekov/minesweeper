#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "ws2812.c"
#include "buttons.c"

enum
{
	BTN_LEFT  = 4,
	BTN_RIGHT = 6,
	BTN_UP    = 1,
	BTN_DOWN  = 9
};

static uint8_t cx, cy;

static void button_event(uint8_t idx)
{
	if(idx == BTN_LEFT)
	{
		if(cx > 0) { --cx; }
	}

	if(idx == BTN_RIGHT)
	{
		if(cx < LED_WIDTH - 1) { ++cx; }
	}

	if(idx == BTN_UP)
	{
		if(cy > 0) { --cy; }
	}

	if(idx == BTN_DOWN)
	{
		if(cy < LED_HEIGHT - 1) { ++cy; }
	}

	led_clear();
	led_pixel(idx % 4, idx / 4, 255, 0, 0);
	led_update();
}

int main(void)
{
	buttons_init();

	led_clear();
	led_update();

	// TODO: Define Board Structure
	// TODO: Generate Board with mines

	cx = 0;
	cy = 0;

	for(;;)
	{
		buttons_scan();

		/*led_clear();
		led_pixel(cx, cy, 0, 0, 255);
		led_update();*/

		// TODO: Check for Cursor input
		// TODO: Handle cursor action (dig/flag)
		// TODO: Update Gamestate
		// TODO: Render Game
	}

	return 0;
}

