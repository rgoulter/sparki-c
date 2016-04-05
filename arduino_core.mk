# For building core.a
# (Just the C parts)
#
# Needs
#  AVR_BIN_DIR
#  ARDUINO_FLAGS
#  CC_FLAGS



# Arduino (the C parts)
ARDUINO_CORE_C_SRCS = WInterrupts.c hooks.c wiring.c wiring_analog.c wiring_digital.c wiring_pulse.c wiring_shift.c
ARDUINO_CORE_C_OBJS = $(ARDUINO_CORE_C_SRCS:.c=.c.o)
BUILT_CORE_C_OBJS = $(foreach obj,$(ARDUINO_CORE_C_OBJS), $(BUILD_DIR)/core/$(obj))



$(BUILD_DIR)/core/%.c.o: $(AVR_DIR)/cores/arduino/%.c $(BUILD_DIR)/core/
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) $< -o $@

$(BUILD_DIR)/core/core.a($(BUILD_DIR)/core/%.c.o): $(BUILD_DIR)/core/%.c.o
	"$(AVR_BIN_DIR)/avr-ar" rcs "$(BUILD_DIR)/core/core.a" $<

$(BUILD_DIR)/core/core.a: $(BUILD_DIR)/core/core.a($(BUILT_CORE_C_OBJS))
