CPU = msp430g2553
CC = msp430-elf-gcc

CFLAGS = -mmcu=$(CPU) -Os -g

all: tamagotchi.elf

tamagotchi.elf: main.c state_transition.s43 lcdutils.c lcddraw.c libTimer.c
	$(CC) $(CFLAGS) -o $@ $^

load: tamagotchi.elf
	msp430-elf-objcopy -O ihex $^ tamagotchi.hex
	mspdebug rf2500 "prog tamagotchi.hex"

clean:
	rm -f *.elf *.hex *.o
