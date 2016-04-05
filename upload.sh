#!/usr/bin/sh

# Run using:
#  ./upload.sh path/to/name.hex

ARDUINO_DIR=/usr/share/arduino
AVR_BIN_DIR=$ARDUINO_DIR/hardware/tools/avr/bin
AVR_DIR=$ARDUINO_DIR/hardware/arduino/avr
SPARKI_DIR=src

CONF=$ARDUINO_DIR/hardware/tools/avr/etc/avrdude.conf
BOARD=atmega32u4
AVRDUDE_PROGRAMMER=avr109
PORT=/dev/ttyACM0
BAUD=57600

$AVR_BIN_DIR/avrdude -C$CONF -v -p$BOARD -c$AVRDUDE_PROGRAMMER -P$PORT -b$BAUD -D -Uflash:w:$1:i
