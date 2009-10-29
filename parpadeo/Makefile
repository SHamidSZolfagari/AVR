MCU = atmega8

TARGET = parpadeo

C_SOURCES = parpadeo.c

S_SOURCES =

DEFINES =

INCLUDES =

LDPATH =

LDLIBS =


DEBUG = stabs

AVRDUDE_FLAGS = -p $(MCU)
AVRDUDE_FLAGS += -c ponyser
AVRDUDE_FLAGS += -P /dev/ttyS0

CFLAGS = -mmcu=$(MCU)
CFLAGS += -g$(DEBUG)
CFLAGS += -std=gnu99
CFLAGS += -Wall
CFLAGS += -Wstrict-prototypes
CFLAGS += -Os
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -Wa,-adhlns=$(<:.c=.lst)
CFLAGS += $(DEFINES) $(INCLUDES)

ASFLAGS = -mmcu=$(MCU)
ASFLAGS += -Wa,-adhlns=$(<:.S=.lst),-g$(DEBUG)
ASFLAGS += $(DEFINES) $(INCLUDES)

LDFLAGS = -mmcu=$(MCU)
LDFLAGS += -Wl,-Map=$(TARGET).map,--cref

ARFLAGS = rcs

OBJCOPY_FLASH_FLAGS = -O ihex
OBJCOPY_FLASH_FLAGS += -R .eeprom
OBJCOPY_FLASH_FLAGS += -R .fuse
OBJCOPY_FLASH_FLAGS += -R .lock

OBJCOPY_EEPROM_FLAGS = -O ihex
OBJCOPY_EEPROM_FLAGS += -j .eeprom
OBJCOPY_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
OBJCOPY_EEPROM_FLAGS += --change-section-lma .eeprom=0
OBJCOPY_EEPROM_FLAGS += --no-change-warnings

AVRDUDE = avrdude
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
NM = avr-nm
AR = avr-ar

OBJ = $(C_SOURCES:.c=.o) $(S_SOURCES:.S=.o)
LST = $(C_SOURCES:.c=.lst) $(S_SOURCES:.S=.lst)
DEP = $(C_SOURCES:.c=.d) $(S_SOURCES:.S=.d)
PRE = $(C_SOURCES:.c=.i) $(S_SOURCES:.S=.i)
ASM = $(C_SOURCES:.c=.s) $(S_SOURCES:.S=.s)

.PHONY : all
.PHONY : build-exe build-lib
.PHONY : dep
.PHONY : pre
.PHONY : asm
.PHONY : elf
.PHONY : lib
.PHONY : hex eep lss sym
.PHONY : burn-flash burn-eeprom
.PHONY : size
.PHONY : clean

.SECONDARY : $(TARGET).elf lib$(TARGET).a

.PRECIOUS : $(OBJ)

all:

build-exe: dep elf hex eep lss sym

build-lib: dep lib

dep: $(DEP)

pre: $(PRE)
asm: $(ASM)

elf: $(TARGET).elf
lib: lib$(TARGET).a

hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss 
sym: $(TARGET).sym


%.elf: $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDPATH) $(LDLIBS) -o $@

%.a : $(OBJ)
	$(AR) $(ARFLAGS) $@ $^


%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o : %.S
	$(CC) $(ASFLAGS) -c -o $@ $< 

%.d : %.c
	$(CC) -MM $(CFLAGS) $< -o $@

%.d : %.S
	$(CC) -MM $(CFLAGS) $< -o $@

%.s : %.c
	$(CC) -S $(CFLAGS) $< -o $@

%.s : %.S
	$(CC) -S $(CFLAGS) $< > $@

%.i : %.c
	$(CC) -E $(CFLAGS) $< -o $@

%.i : %.S
	$(CC) -E $(CFLAGS) $< -o $@

%.hex: %.elf
	$(OBJCOPY) $(OBJCOPY_FLASH_FLAGS) $< $@

%.eep: %.elf
	$(OBJCOPY) $(OBJCOPY_EEPROM_FLAGS) $< $@

%.lss: %.elf
	$(OBJDUMP) -h -S $< > $@

%.sym: %.elf
	$(NM) -n $< > $@


burn-flash: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$<

burn-eeprom: $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U eeprom:w:$<


size: $(TARGET).elf
	$(SIZE) $<


clean:
	$(RM) $(TARGET).hex
	$(RM) $(TARGET).eep
	$(RM) $(TARGET).map
	$(RM) $(TARGET).sym
	$(RM) $(TARGET).lss
	$(RM) $(TARGET).elf
	$(RM) lib$(TARGET).a
	$(RM) $(OBJ)
	$(RM) $(LST)
	$(RM) $(DEP)
	$(RM) $(PRE)
	$(RM) $(ASM)

-include $(wildcard *.d)
