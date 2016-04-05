# Rules for the examples
#
# May be missing some, since haven't yet implemented those
# features.

# Not sure if more idiomatic way to do this. :/

$(BUILD_DIR)/Edge_Avoidance.c.o:  examples/Examples/Edge_Avoidance/Edge_Avoidance.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/Follow_Light.c.o:    examples/Examples/Follow_Light/Follow_Light.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/Avoid_Light.c.o:     examples/Examples/Avoid_Light/Avoid_Light.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/Line_Following.c.o:  examples/Examples/Line_Following/Line_Following.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/Restore_Default.c.o: examples/Examples/Restore_Default/Restore_Default.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/Wall_Avoidance.c.o:  examples/Examples/Wall_Avoidance/Wall_Avoidance.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/Hill_Climb.c.o:      examples/Examples/Hill_Climb/Hill_Climb.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@

$(BUILD_DIR)/Wheels.c.o:                  examples/Parts/Wheels/Wheels.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/Status_LED.c.o:              examples/Parts/Status_LED/Status_LED.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/Remote.c.o:                  examples/Parts/Remote/Remote.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/Servo.c.o:                   examples/Parts/Servo/Servo.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/LCD_Bitmap.c.o:              examples/Parts/LCD/examples/LCD_Bitmap/LCD_Bitmap.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/LCD_Circles.c.o:             examples/Parts/LCD/examples/LCD_Circles/LCD_Circles.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/LCD_Pixels.c.o:              examples/Parts/LCD/examples/LCD_Pixels/LCD_Pixels.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/LCD_Print.c.o:               examples/Parts/LCD/examples/LCD_Print/LCD_Print.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/LCD_Rectangles.c.o:          examples/Parts/LCD/examples/LCD_Rectangles/LCD_Rectangles.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/LCD_Lines.c.o:               examples/Parts/LCD/examples/LCD_Lines/LCD_Lines.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/LCD_Text.c.o:                examples/Parts/LCD/examples/LCD_Text/LCD_Text.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/Infrared_Reflectance.c.o:    examples/Parts/Infrared_Reflectance/Infrared_Reflectance.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/Light.c.o:                   examples/Parts/Light/Light.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/RGB_Fading.c.o:              examples/Parts/RGB/examples/RGB_Fading/RGB_Fading.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/RGB_Rainbow.c.o:             examples/Parts/RGB/examples/RGB_Rainbow/RGB_Rainbow.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/RGB_Basic.c.o:               examples/Parts/RGB/examples/RGB_Basic/RGB_Basic.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/Infrared_LED.c.o:            examples/Parts/Infrared_LED/Infrared_LED.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
$(BUILD_DIR)/Ultrasonic_Range_Finder.c.o: examples/Parts/Ultrasonic_Range_Finder/Ultrasonic_Range_Finder.c
	"$(AVR_BIN_DIR)/avr-gcc" $(CC_FLAGS) $(ARDUINO_FLAGS) "-Isrc/" $< -o $@
