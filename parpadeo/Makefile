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

CFLAGS          = -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
LDFLAGS         = -Wl,-Map,$(PRG).map

.PHONY: all clean program-flash program-eeprom

all: $(PRG).hex

%.elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@


program-flash: %.hex
	$(ISP) -p $(ISP_PARTNO) -P $(ISP_PORT) -c $(ISP_PROGRAMMER) -U flash:w:$<

program-eeprom: %_eeprom.hex
	$(ISP) -p $(ISP_PARTNO) -P $(ISP_PORT) -c $(ISP_PROGRAMMER) -U eeprom:w:$<

clean:
	rm -rf *.o *.elf *.lst *.map *.hex
