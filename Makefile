#
# Makefile for Sparki projects,
# derived from the verbose output of the Arduino IDE
#

ARDUINO_DIR = /usr/share/arduino
AVR_BIN_DIR = $(ARDUINO_DIR)/hardware/tools/avr/bin
AVR_DIR     = $(ARDUINO_DIR)/hardware/arduino/avr
SPARKI_DIR  = src

INCLUDES = "-I$(AVR_DIR)/cores/arduino" \
           "-I$(AVR_DIR)/variants/leonardo"
ARDUINO_FLAGS = -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=10608 \
                -DARDUINO_AVR_LEONARDO -DARDUINO_ARCH_AVR \
                -DUSB_VID=0x2341 -DUSB_PID=0x8036 \
                '-DUSB_MANUFACTURER="Unknown"' '-DUSB_PRODUCT="Arduino Leonardo"' \
                $(INCLUDES)
CC_FLAGS  = -c -g -Os -w -std=gnu11 -ffunction-sections -fdata-sections -MMD
CPP_FLAGS = -c -g -Os -w -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD

BUILD_DIR = build



SPARKI_C_SRCS = CSPI.c
SPARKI_C_OBJS = $(SPARKI_C_SRCS:.c=.c.o)
BUILT_SPARKI_C_OBJS = $(foreach obj, $(SPARKI_C_OBJS), $(BUILD_DIR)/$(obj))

SPARKI_CPP_SRCS = Sparki.cpp SparkiWire.cpp
SPARKI_CPP_OBJS = $(SPARKI_CPP_SRCS:.cpp=.cpp.o)
BUILT_SPARKI_CPP_OBJS = $(foreach obj, $(SPARKI_CPP_OBJS), $(BUILD_DIR)/$(obj))



all: $(BUILD_DIR)/sparki.a

objects: $(BUILT_SPARKI_CPP_OBJS)

# Ensure the output dirs we need are there
$(BUILD_DIR)/:
	mkdir -p $@



# Sparki
$(BUILD_DIR)/%.c.o: $(SPARKI_DIR)/%.c $(BUILD_DIR)/
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-I$(SPARKI_DIR)" $< -o $@

$(BUILD_DIR)/%.cpp.o: $(SPARKI_DIR)/%.cpp $(BUILD_DIR)/
	"$(AVR_BIN_DIR)/avr-g++" $(CPP_FLAGS) $(ARDUINO_FLAGS) "-I$(SPARKI_DIR)" $< -o $@



$(BUILD_DIR)/sparki.a($(BUILD_DIR)/%.c.o): $(BUILD_DIR)/%.c.o
	"$(AVR_BIN_DIR)/avr-ar" rcs "$(BUILD_DIR)/sparki.a" $<

$(BUILD_DIR)/sparki.a($(BUILD_DIR)/%.cpp.o): $(BUILD_DIR)/%.cpp.o
	"$(AVR_BIN_DIR)/avr-ar" rcs "$(BUILD_DIR)/sparki.a" $<

$(BUILD_DIR)/sparki.a: $(BUILD_DIR)/sparki.a($(BUILT_SPARKI_C_OBJS)) $(BUILD_DIR)/sparki.a($(BUILT_SPARKI_CPP_OBJS))



clean:
	@rm $(BUILD_DIR)/sparki.a 2> /dev/null || true
	@rm $(BUILT_SPARKI_C_OBJS) 2> /dev/null || true
	@rm $(BUILT_SPARKI_C_OBJS:.o=.d) 2> /dev/null || true
	@rm $(BUILT_SPARKI_CPP_OBJS) 2> /dev/null || true
	@rm $(BUILT_SPARKI_CPP_OBJS:.o=.d) 2> /dev/null || true

contents:
	$(AVR_BIN_DIR)/avr-ar t $(BUILD_DIR)/sparki.a

.PHONY: clean contents
