MCU    = atmega328p
F_CPU  = 16000000
TARGET = main
SRCS   = main.c
OPT    = s

CC      = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE    = avr-size

CFLAGS = \
	-DF_CPU=$(F_CPU) \
	-O$(OPT) \
	-ffreestanding \
	-funsigned-char \
	-funsigned-bitfields \
	-fpack-struct \
	-fshort-enums \
	-Wall \
	-Wno-array-bounds \
	-Wstrict-prototypes \
	-Wa,-adhlns=$(<:.c=.lst) \
	-std=gnu99 \
	-mmcu=$(MCU)

all: clean $(TARGET).elf size

$(TARGET).elf: main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET).elf

size: $(TARGET).elf
	$(SIZE) -C --mcu=$(MCU) $(TARGET).elf

clean:
	rm -f main.elf

connect:
	avrdude -c arduino -P /dev/ttyACM0 -p m328p

flash:
	avrdude -c arduino -P /dev/ttyACM0 -p m328p -U main.elf
