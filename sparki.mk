# 
#
# Needs
#  AVR_BIN_DIR for avr-gcc, avr-ar
#  SPARKI_DIR set to location of C Sparki srcs
#  BUILD_DIR
# and directories:
#  BUILD_DIR
#  BUILD_DIR/sparki

# Accel,Mag,LCD,Servo,IR,PingSingle,Motor,Beep,IRReflectance,RGB
SPARKI_C_PARTS = LCD Servo IR PingSingle Motor Beep IRReflectance RGB
SPARKI_C_SRCS = CSPI.c Sparki.c $(foreach part, $(SPARKI_C_PARTS), SparkiLib$(part).c)
SPARKI_C_OBJS = $(SPARKI_C_SRCS:.c=.c.o)
BUILT_SPARKI_C_OBJS = $(foreach obj, $(SPARKI_C_OBJS), $(BUILD_DIR)/sparki/$(obj))


# Sparki
$(BUILD_DIR)/sparki/%.c.o: $(SPARKI_DIR)/%.c $(BUILD_DIR)/sparki/
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-I$(SPARKI_DIR)" $< -o $@


$(BUILD_DIR)/sparki.a($(BUILD_DIR)/sparki/%.c.o): $(BUILD_DIR)/sparki/%.c.o
	"$(AVR_BIN_DIR)/avr-ar" rcs "$(BUILD_DIR)/sparki.a" $<

$(BUILD_DIR)/sparki.a: $(BUILD_DIR)/sparki.a($(BUILT_SPARKI_C_OBJS))
