PRG             = parpadeo
OBJ             = $(PRG).o
MCU_TARGET      = atmega8

ISP_PARTNO      = m8
ISP_PORT        = /dev/ttyS0
ISP_PROGRAMMER  = ponyser
ISP             = avrdude

DEFS            =
LIBS            =

OPTIMIZE        = -O2

CC              = avr-gcc
LD              = avd-gcc
OBJCOPY         = avr-objcopy
OBJDUMP         = avr-objdump
SIZE            = avr-size

CFLAGS          = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
LDFLAGS         =

.PHONY: all clean program-flash

all: $(PRG).hex

%.elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(SIZE) $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@


program-flash: $(PRG).hex
	$(ISP) -p $(ISP_PARTNO) -P $(ISP_PORT) -c $(ISP_PROGRAMMER) -U flash:w:$<

clean:
	rm -rf *.o *.elf *.lst *.hex
