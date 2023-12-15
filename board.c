static uint8_t field[LED_PIXELS];

static uint8_t getField(uint8_t x, uint8_t y)
{
	return field[y * LED_WIDTH + x];
}

static void setField(uint8_t x, uint8_t y, uint8_t v)
{
	field[y * LED_WIDTH + x] = v;
}

static uint8_t hasMine(uint8_t x, uint8_t y)
{
	return getField(x, y) == 0xFF;
}

static void revealField(uint8_t x, uint8_t y)
{
	uint8_t f;
	if(hasMine(x, y)) { return; }

	f = getField(x, y);
	if(f <= 0x04)
	{
		setField(x, y, f + 0xF0);
	}
}

static uint8_t revealRecursive(uint8_t x, uint8_t y)
{
	if(x >= LED_WIDTH || y >= LED_HEIGHT)
	{
		return 0;
	}

	uint8_t prev = getField(x, y);
	if(hasMine(x, y))
	{
		return 1;
	}

	revealField(x, y);

	if(prev == 0x00)
	{
		revealRecursive(x - 1, y);
		revealRecursive(x + 1, y);
		revealRecursive(x, y - 1);
		revealRecursive(x, y + 1);
	}

	return 0;
}

static void revealAllFields(void)
{
	uint8_t x, y;
	for(x = 0; x < LED_WIDTH; ++x)
	{
		for(y = 0; y < LED_HEIGHT; ++y)
		{
			revealField(x, y);
		}
	}
}

static void incrementCounter(uint8_t x, uint8_t y)
{
	// don't increment if outside of field (because overflow)
	if(x >= LED_WIDTH) return;
	if(y >= LED_HEIGHT) return;

	// don't increment on mine
	if(hasMine(x, y)) return;

	field[y * LED_WIDTH + x]++;
}

static void placeMine(uint8_t x, uint8_t y)
{
	field[y * LED_WIDTH + x] = 0xFF;
}

static uint8_t tryPlaceMine(uint8_t x, uint8_t y)
{
	if(hasMine(x, y)) return 0;
	placeMine(x, y);
	return 1;
}

static void generateField(int c)
{
	memset(field, 0, sizeof(field));
	
	for (uint8_t i = 0; i < c; ++i)
	{
		uint8_t r = rand() % 100;
		uint8_t x = r % LED_WIDTH;
		uint8_t y = r / LED_WIDTH;

		while(!tryPlaceMine(x, y)) {}
	}

	for (uint8_t x = 0; x < LED_WIDTH; ++x)
	{
		for (uint8_t y = 0; y < LED_HEIGHT; ++y)
		{
			if (!hasMine(x, y)) continue;

			// top
			incrementCounter(x - 1, y - 1);
			incrementCounter(x, y - 1);
			incrementCounter(x + 1, y - 1);
			// site
			incrementCounter(x - 1, y);
			incrementCounter(x + 1, y);
			// bottom
			incrementCounter(x - 1, y + 1);
			incrementCounter(x, y + 1);
			incrementCounter(x + 1, y + 1);
		}
	}
}

static void displayField(void)
{
	for (uint8_t x = 0; x < LED_WIDTH; ++x)
	{
		for (uint8_t y = 0; y < LED_HEIGHT; ++y)
		{
			uint8_t f = getField(x, y);
			switch (f)
			{
				case 0xF0:    // 0 mines (shown)
					led_pixel(x, y, 0, 0, 0);
					break;
				case 0xF1:    // 1 mine (shown)
					led_pixel(x, y, 0, 255, 0);
					break;
				case 0xF2:    // 2 mines (shown)
					led_pixel(x, y, 255, 255, 0);
					break;
				case 0xF3:    // 3 mines (shown)
					led_pixel(x, y, 255, 127, 0);
					break;
				case 0xF4:    // 4 mines (shown)
					led_pixel(x, y, 255, 0, 0);
					break;
				default:    // field not opened (0-4 mines)
					led_pixel(x, y, 255, 255, 255);
					break;
			}
		}
	}
}
