MCU = atmega8

TARGET = led-matrix
SRC = $(TARGET).c
ASRC =

CDEFS =

CINCS =

LIBS = 

AVRDUDE = avrdude
AVRDUDE_PROGRAMMER = ponyser
AVRDUDE_PORT = /dev/ttyS0

CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
NM = avr-nm

CFLAGS = -mmcu=$(MCU) -I.
CFLAGS += -gstabs
CFLAGS += $(CDEFS) $(CINCS)
CFLAGS += -Os
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -Wall -Wstrict-prototypes
CFLAGS += -Wa,-adhlns=$(<:.c=.lst)
CFLAGS += -std=gnu99

ASFLAGS = -mmcu=$(MCU) -I.
ASFLAGS += -Wa,-adhlns=$(<:.S=.lst),-gstabs 

LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#LDFLAGS += -Wl,-Tdata=0x801100,--defsym=__heap_end=0x80ffff
# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#LDFLAGS += -Wl,--defsym=__heap_start=0x801100,--defsym=__heap_end=0x80ffff
#LDFLAGS += -Wl,-u,vfprintf -lprintf_min
#LDFLAGS += -Wl,-u,vfscanf -lscanf_min
#LDFLAGS += -Wl,-u,vfprintf -lprintf_flt
#LDFLAGS += -Wl,-u,vfscanf -lscanf_flt
#LDFLAGS += -lm
LDFLAGS += $(LIBS)

OBJ = $(SRC:.c=.o) $(ASRC:.S=.o) 
LST = $(SRC:.c=.lst) $(ASRC:.S=.lst)

.PHONY : all deps build elf hex eep lss sym coff extcoff clean \
	program-flash program-eeprom size-elf size-hex
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)

all: deps build

deps: $(SRC:.c=.d)

build: elf hex eep lss sym

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss 
sym: $(TARGET).sym


%.elf: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) --output $@ $(LDFLAGS)

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@ 

%.s : %.c
	$(CC) -S $(CFLAGS) $< -o $@

%.i : %.c
	$(CC) -E $(CFLAGS) $< -o $@

%.d : %.c
	$(CC) -M $(CFLAGS) $< -o $@

%.o : %.S
	$(CC) -c $(ASFLAGS) $< -o $@

%.hex: %.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

%.eep: %.elf
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O ihex $< $@

%.lss: %.elf
	$(OBJDUMP) -h -S $< > $@

%.sym: %.elf
	$(NM) -n $< > $@


coff: $(TARGET).elf
	$(OBJCOPY) --debugging \
	--change-section-address .data-0x800000 \
	--change-section-address .bss-0x800000 \
	--change-section-address .noinit-0x800000 \
	--change-section-address .eeprom-0x810000 
	-O coff-avr $< $(TARGET).cof

extcoff: $(TARGET).elf
	$(OBJCOPY) --debugging \
	--change-section-address .data-0x800000 \
	--change-section-address .bss-0x800000 \
	--change-section-address .noinit-0x800000 \
	--change-section-address .eeprom-0x810000 
	-O coff-ext-avr $< $(TARGET).cof


size-hex: $(TARGET).hex
	$(SIZE) --target=ihex $(TARGET).hex

size-elf: $(TARGET).elf
	$(SIZE) $(TARGET).elf

program-flash: $(TARGET).hex
	$(AVRDUDE) -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) \
	-U flash:w:$<

program-eeprom: $(TARGET).eep
	$(AVRDUDE) -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) \
	-U eeprom:w:$<


clean:
	$(RM) $(TARGET).hex
	$(RM) $(TARGET).eep
	$(RM) $(TARGET).obj
	$(RM) $(TARGET).cof
	$(RM) $(TARGET).elf
	$(RM) $(TARGET).map
	$(RM) $(TARGET).obj
	$(RM) $(TARGET).sym
	$(RM) $(TARGET).lss
	$(RM) $(OBJ)
	$(RM) $(LST)
	$(RM) $(SRC:.c=.s)
	$(RM) $(SRC:.c=.i)
	$(RM) $(SRC:.c=.d)


-include $(wildcard *.d)
