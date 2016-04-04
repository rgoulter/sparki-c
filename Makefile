#
# Makefile for Sparki projects,
# derived from the verbose output of the Arduino IDE
#

ARDUINO_DIR = /usr/share/arduino
AVR_BIN_DIR = $(ARDUINO_DIR)/hardware/tools/avr/bin
AVR_DIR     = $(ARDUINO_DIR)/hardware/arduino/avr
SPARKI_DIR  = src

SPARKI_FLAGS = -DNO_ACCEL -DNO_MAG
INCLUDES = "-I$(AVR_DIR)/cores/arduino" \
           "-I$(AVR_DIR)/variants/leonardo"
ARDUINO_FLAGS = -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=10608 \
                -DARDUINO_AVR_LEONARDO -DARDUINO_ARCH_AVR \
                -DUSB_VID=0x2341 -DUSB_PID=0x8036 \
                '-DUSB_MANUFACTURER="Unknown"' '-DUSB_PRODUCT="Arduino Leonardo"' \
                $(INCLUDES)
CC_FLAGS  = -c -g -Os -Wall -std=gnu11 -ffunction-sections -fdata-sections -MMD $(SPARKI_FLAGS)
CPP_FLAGS = -c -g -Os -w -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD

BUILD_DIR = build



# Accel,Mag,LCD,Servo,IR,PingSingle,Motor,Beep,IRReflectance,RGB
SPARKI_C_PARTS = LCD Servo IR PingSingle Motor Beep IRReflectance RGB
SPARKI_C_SRCS = CSPI.c Sparki.c $(foreach part, $(SPARKI_C_PARTS), SparkiLib$(part).c)
SPARKI_C_OBJS = $(SPARKI_C_SRCS:.c=.c.o)
BUILT_SPARKI_C_OBJS = $(foreach obj, $(SPARKI_C_OBJS), $(BUILD_DIR)/sparki/$(obj))

# SparkiWire.cpp
SPARKI_CPP_SRCS =
SPARKI_CPP_OBJS = $(SPARKI_CPP_SRCS:.cpp=.cpp.o)
BUILT_SPARKI_CPP_OBJS = $(foreach obj, $(SPARKI_CPP_OBJS), $(BUILD_DIR)/sparki/(obj))



all: $(BUILD_DIR)/sparki.a

objects: $(BUILT_SPARKI_CPP_OBJS)

# Ensure the output dirs we need are there
$(BUILD_DIR)/:
	mkdir -p $@

$(BUILD_DIR)/sparki/:
	mkdir -p $@

$(BUILD_DIR)/core/:
	mkdir -p $@



# Sparki
$(BUILD_DIR)/sparki/%.c.o: $(SPARKI_DIR)/%.c $(BUILD_DIR)/sparki/
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-I$(SPARKI_DIR)" $< -o $@

$(BUILD_DIR)/sparki/%.cpp.o: $(SPARKI_DIR)/%.cpp $(BUILD_DIR)/sparki/
	"$(AVR_BIN_DIR)/avr-g++" $(CPP_FLAGS) $(ARDUINO_FLAGS) "-I$(SPARKI_DIR)" $< -o $@



$(BUILD_DIR)/sparki.a($(BUILD_DIR)/sparki/%.c.o): $(BUILD_DIR)/sparki/%.c.o
	"$(AVR_BIN_DIR)/avr-ar" rcs "$(BUILD_DIR)/sparki.a" $<

$(BUILD_DIR)/sparki.a($(BUILD_DIR)/sparki/%.cpp.o): $(BUILD_DIR)/sparki/%.cpp.o
	"$(AVR_BIN_DIR)/avr-ar" rcs "$(BUILD_DIR)/sparki.a" $<

$(BUILD_DIR)/sparki.a: $(BUILD_DIR)/sparki.a($(BUILT_SPARKI_C_OBJS))



# Arduino (the C parts)
ARDUINO_CORE_C_SRCS = WInterrupts.c hooks.c wiring.c wiring_analog.c wiring_digital.c wiring_pulse.c wiring_shift.c
ARDUINO_CORE_C_OBJS = $(ARDUINO_CORE_C_SRCS:.c=.c.o)
BUILT_CORE_C_OBJS = $(foreach obj,$(ARDUINO_CORE_C_OBJS), $(BUILD_DIR)/core/$(obj))

$(BUILD_DIR)/core/%.c.o: $(AVR_DIR)/cores/arduino/%.c $(BUILD_DIR)/core/
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) $< -o $@

$(BUILD_DIR)/core/core.a($(BUILD_DIR)/core/%.c.o): $(BUILD_DIR)/core/%.c.o
	"$(AVR_BIN_DIR)/avr-ar" rcs "$(BUILD_DIR)/core/core.a" $<

$(BUILD_DIR)/core/core.a: $(BUILD_DIR)/core/core.a($(BUILT_CORE_C_OBJS))


# Example program
NAME=LCD_Text
$(BUILD_DIR)/$(NAME).c.o: $(NAME).c $(BUILD_DIR)/
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@


$(BUILD_DIR)/$(NAME).elf: $(BUILD_DIR)/core/core.a $(BUILD_DIR)/sparki.a $(BUILD_DIR)/$(NAME).c.o
	"$(AVR_BIN_DIR)/avr-gcc" -w -Os -Wl,--gc-sections -mmcu=atmega32u4 -o $@ "$(BUILD_DIR)/$(NAME).c.o" $(BUILD_DIR)/sparki.a "$(BUILD_DIR)/core/core.a" "-L$(BUILD_DIR)" -lm

# .eeprom section (not uploaded)
$(BUILD_DIR)/$(NAME).eep: $(BUILD_DIR)/$(NAME).elf
	"$(AVR_BIN_DIR)/avr-objcopy" -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 "$(BUILD_DIR)/$(NAME).elf" "$(BUILD_DIR)/$(NAME).eep"

# Intel Hex format
$(BUILD_DIR)/$(NAME).hex: $(BUILD_DIR)/$(NAME).elf
	"$(AVR_BIN_DIR)/avr-objcopy" -O ihex -R .eeprom "$(BUILD_DIR)/$(NAME).elf" "$(BUILD_DIR)/$(NAME).hex"

# was -cavr109
# try -cavrisp2
AVRDUDE_PROGRAMMER=avr109
upload: $(BUILD_DIR)/$(NAME).hex
	$(AVR_BIN_DIR)/avrdude -C$(ARDUINO_DIR)/hardware/tools/avr/etc/avrdude.conf -v -patmega32u4 -c$(AVRDUDE_PROGRAMMER) -P/dev/ttyACM0 -b57600 -D -Uflash:w:$(BUILD_DIR)/$(NAME).hex:i





clean:
	@rm $(BUILD_DIR)/sparki.a 2> /dev/null || true
	@rm $(BUILT_SPARKI_C_OBJS) 2> /dev/null || true
	@rm $(BUILT_SPARKI_C_OBJS:.o=.d) 2> /dev/null || true
	@rm $(BUILT_SPARKI_CPP_OBJS) 2> /dev/null || true
	@rm $(BUILT_SPARKI_CPP_OBJS:.o=.d) 2> /dev/null || true

contents:
	$(AVR_BIN_DIR)/avr-ar t $(BUILD_DIR)/sparki.a

.PHONY: clean contents
