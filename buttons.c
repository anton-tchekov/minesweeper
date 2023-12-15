#define BUTTON_DEBOUNCE 15

static void button_event(uint8_t idx);

static uint8_t states[16];

static void buttons_init(void)
{
	PORTB |= 0x0F;
}

static void buttons_scan(void)
{
	uint8_t row, col, val, idx;
	idx = 0;
	for(row = 0; row < 4; ++row)
	{
		DDRC |= (1 << row);
		_delay_ms(1);
		val = PINB & 0x0F;
		DDRC &= (1 << row);

		for(col = 0; col < 4; ++col, ++idx)
		{
			if(val & (1 << col))
			{
				states[idx] = 0;
			}
			else
			{
				if(states[idx] < BUTTON_DEBOUNCE)
				{
					++states[idx];
				}
				else
				{
					states[idx] = 0;
					button_event(idx);
				}
			}
		}
	}
}
