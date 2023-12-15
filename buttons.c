static void button_event(uint8_t idx);

static void buttons_init(void)
{
	PORTC |= 0x3F;
}

static void buttons_scan(void)
{
	uint8_t val, btn;
	val = PINC;
	for(btn = 0; btn < 6; ++btn)
	{
		if(!(val & (1 << btn)))
		{
			button_event(btn);
		}
	}
}
