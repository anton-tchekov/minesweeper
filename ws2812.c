#define LED_WIDTH       10
#define LED_HEIGHT      10
#define LED_PIXELS        (LED_WIDTH * LED_HEIGHT)
#define LED_BYTES         (3 * LED_PIXELS)

#define WS2812_OUT        PORTD
#define WS2812_DIR        DDRD
#define WS2812_PIN        PD2

#define W_ZERO_PULSE    350
#define W_ONE_PULSE     900
#define W_TOTAL_PERIOD 1250
#define W_FIXED_LOW       2
#define W_FIXED_HIGH      4
#define W_FIXED_TOTAL     8

#define W_ZERO_CYCLES      (((F_CPU / 1000) * W_ZERO_PULSE) / 1000000)
#define W_ONE_CYCLES       (((F_CPU / 1000) * W_ONE_PULSE + 500000) / 1000000)
#define W_TOTAL_CYCLES     (((F_CPU / 1000) * W_TOTAL_PERIOD + 500000) / 1000000)

#define W_1                (W_ZERO_CYCLES - W_FIXED_LOW)
#define W_2                (W_ONE_CYCLES - w_fixedhigh - W_1)
#define W_3                (W_TOTAL_CYCLES - W_FIXED_TOTAL - W_1 - W_2)

#if (W_1 > 0)
#define W_1_NOPS W_1
#else
#define W_1_NOPS  0
#endif

#define w_lowtime (((W_1_NOPS + W_FIXED_LOW) * 1000000) / (F_CPU / 1000))

#if (w_lowtime > 550)
#error "F_CPU to low"
#elif (w_lowtime > 450)
#warning "Critical timing"
#endif

#if (W_2 > 0)
#define W_2_NOPS W_2
#else
#define W_2_NOPS 0
#endif

#if (W_3 > 0)
#define W_3_NOPS W_3
#else
#define W_3_NOPS 0
#endif

#define W_NOP_1  "nop      \n\t"
#define W_NOP_2  "rjmp .+0 \n\t"
#define W_NOP_4  W_NOP_2 W_NOP_2
#define W_NOP_8  W_NOP_4 W_NOP_4
#define W_NOP_16 W_NOP_8 W_NOP_8

static uint8_t _pixels[LED_BYTES];

static void _ws2812(uint8_t *pixels, uint16_t count)
{
	uint8_t b, c, h, l, s;
	h = (1 << WS2812_PIN);
	WS2812_DIR |= h;
	l = ~h & WS2812_OUT;
	h |= WS2812_OUT;
	s = SREG;
	asm volatile ("cli");
	while(count--)
	{
		b = *pixels++;
		asm volatile
		(
			"       ldi   %0,8  \n\t"
			"loop%=:            \n\t"
			"       out   %2,%3 \n\t"
#if (W_1_NOPS & 1)
	W_NOP_1
#endif
#if (W_1_NOPS & 2)
	W_NOP_2
#endif
#if (W_1_NOPS & 4)
	W_NOP_4
#endif
#if (W_1_NOPS & 8)
	W_NOP_8
#endif
#if (W_1_NOPS & 16)
	W_NOP_16
#endif
			"       sbrs  %1,7  \n\t"
			"       out   %2,%4 \n\t"
			"       lsl   %1    \n\t"
#if (W_2_NOPS & 1)
	W_NOP_1
#endif
#if (W_2_NOPS & 2)
	W_NOP_2
#endif
#if (W_2_NOPS & 4)
	W_NOP_4
#endif
#if (W_2_NOPS & 8)
	W_NOP_8
#endif
#if (W_2_NOPS & 16)
	W_NOP_16
#endif
			"       out   %2,%4 \n\t"
#if (W_3_NOPS & 1)
	W_NOP_1
#endif
#if (W_3_NOPS & 2)
	W_NOP_2
#endif
#if (W_3_NOPS & 4)
	W_NOP_4
#endif
#if (W_3_NOPS & 8)
	W_NOP_8
#endif
#if (W_3_NOPS & 16)
	W_NOP_16
#endif
			"       dec   %0    \n\t"
			"       brne  loop%=\n\t"
			: "=&d" (c)
			: "r" (b),
			"I" (_SFR_IO_ADDR(WS2812_OUT)),
			"r" (h),
			"r" (l)
		);
	}

	SREG = s;
}

static void led_pixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b)
{
	uint16_t i;
	x = (LED_WIDTH - 1) - x;
	y = (LED_HEIGHT - 1) - y;
	i = 3 * ((y % 2) ? ((LED_WIDTH * y) + ((LED_WIDTH - 1) - x)) :
		(LED_WIDTH * y + x));

	_pixels[i] = g;
	_pixels[++i] = r;
	_pixels[++i] = b;
}

static void led_clear(void)
{
	memset(_pixels, 0, LED_BYTES);
}

static void led_update(void)
{
	_ws2812(_pixels, LED_BYTES);
}
