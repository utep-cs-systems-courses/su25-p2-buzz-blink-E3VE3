COMMON_OBJECTS = toyCo.o
CPU = msp430g2553
CFLAGS = -mmcu=$(CPU) -I../h
LDFLAGS = -L/opt/ti/msp430_gcc/include

CC = msp430-elf-gcc
AS = msp430-elf-as

all: toyCo.elf

toyCo.elf: $(COMMON_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ ../lib/libTimer.a

load: toyCo.elf
	msp430loader.sh toyCo.elf

clean:
	rm -f *.elf *.o

