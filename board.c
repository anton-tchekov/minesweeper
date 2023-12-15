static uint8_t field[LED_PIXELS];

uint8_t getField(int x, int y)
{
	return field[y * LED_WIDTH + x];
}

uint8_t setField(int x, int y, uint8_t v) {
	field[y * LED_WIDTH + x] = v;
}

uint8_t hasMine(int x, int y)
{
	return getField(x, y) == 0xFF;
}

void revealField(int x, int y) {
	if(hasMine(x, y)) return;

	int f = getField(x, y);

	if(f >= 0x01 && f <= 0x04) {
		setField(x, y, f + 0xF0);
	}
}
void revealAllFields() {
	for (int x = 0; x < LED_WIDTH; ++x)
	{
		for (int y = 0; y < LED_HEIGHT; ++y)
		{
			revealField(x, y);
		}
	}
}

void incrementCounter(int x, int y)
{
	// don't increment on mine
	if (hasMine(x, y)) return;
	// don't increment if outside of field (because overflow)
	if (x < 0 || x > LED_WIDTH) return;
	if (y < 0 || y > LED_HEIGHT) return;

	field[y * LED_WIDTH + x]++;
}

void placeMine(int x, int y)
{
	field[y * LED_WIDTH + x] = 0xFF;
}


uint8_t tryPlaceMine(int x, int y)
{
	if (hasMine(x, y)) return 0;
	placeMine(x, y);
	return 1;
}

void generateField(int c)
{
	for (int i = 0; i < c; ++i)
	{
		int r = rand() % 100;
		int x = r % LED_WIDTH;
		int y = r / LED_WIDTH;

		while (!tryPlaceMine(x, y))
		{}
	}

	for (int x = 0; x < LED_WIDTH; ++x)
	{
		for (int y = 0; y < LED_HEIGHT; ++y)
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

void displayField()
{
	for (int x = 0; x < LED_WIDTH; ++x)
	{
		for (int y = 0; y < LED_HEIGHT; ++y)
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