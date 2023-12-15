#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "ws2812.c"
#include "board.c"
#include "adc.c"
#include "buttons.c"

#define NUM_MINES 10

enum
{
	BTN_LEFT   = 0,
	BTN_RIGHT  = 3,
	BTN_UP     = 2,
	BTN_DOWN   = 1,
	BTN_REVEAL = 4,
	BTN_FLAG   = 5
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

	if(idx == BTN_REVEAL)
	{
		if(revealRecursive(cx, cy))
		{
			for(uint8_t i = 0; i < 3; ++i)
			{
				led_clear();
				for(uint8_t y = 0; y < LED_HEIGHT; ++y)
				{
					for(uint8_t x = 0; x < LED_WIDTH; ++x)
					{
						led_pixel(x, y, 255, 0, 0);
					}
				}

				led_update();
				_delay_ms(100);
				led_clear();
				led_update();
				_delay_ms(100);
			}

			generateField(NUM_MINES);
		}
	}
}

int main(void)
{
	uint8_t blink = 0;
	
	buttons_init();

	led_clear();
	led_update();

	adc_init();
	srand(adc_read(ADC_CHANNEL_NOISE));

	generateField(NUM_MINES);

	cx = 0;
	cy = 0;

	for(;;)
	{
		buttons_scan();

		led_clear();
		displayField();
		
		if(blink)
		{
			led_pixel(cx, cy, 0, 0, 255);
		}

		blink = !blink;

		led_update();

		_delay_ms(200);

		// TODO: Check for Cursor input
		// TODO: Handle cursor action (dig/flag)
		// TODO: Update Gamestate
		// TODO: Render Game
	}

	return 0;
}
