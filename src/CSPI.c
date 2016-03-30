/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "pins_arduino.h"
#include "CSPI.h"

void SPI_begin() {
  // Set direction register for SCK and MOSI pin.
  // MISO pin automatically overrides to INPUT.
  // When the SS pin is set as OUTPUT, it can be used as
  // a general purpose output port (it doesn't influence
  // SPI operations).

  // from pins_arduino.h:
  // SCK  = digital 15 = PB1
  // MOSI = digital 16 = PB2
  // SS   = digital 17 = PB0

  // pinMode(SCK, OUTPUT); // pinMode(MOSI, OUTPUT); // pinMode(SS, OUTPUT);
  DDRB |= _BV(PORTB1) | _BV(PORTB2) | _BV(PORTB0);

  // digitalWrite(SCK, LOW);
  // digitalWrite(MOSI, LOW);
  PORTB &= ~(_BV(PORTB1) | _BV(PORTB2));
  // digitalWrite(SS, HIGH);
  PORTB |= _BV(PORTB0);

  // Warning: if the SS pin ever becomes a LOW INPUT then SPI
  // automatically switches to Slave, so the data direction of
  // the SS pin MUST be kept as OUTPUT.
  SPCR |= _BV(MSTR);
  SPCR |= _BV(SPE);
}

void SPI_end() {
  SPCR &= ~_BV(SPE);
}

void SPI_setBitOrder(uint8_t bitOrder)
{
  // LSBFIRST == 0.
  // (But we don't use this among these files, so?).
  if(bitOrder == 0) {
    SPCR |= _BV(DORD);
  } else {
    SPCR &= ~(_BV(DORD));
  }
}

void SPI_setDataMode(uint8_t mode)
{
  SPCR = (SPCR & ~SPI_MODE_MASK) | mode;
}

void SPI_setClockDivider(uint8_t rate)
{
  SPCR = (SPCR & ~SPI_CLOCK_MASK) | (rate & SPI_CLOCK_MASK);
  SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((rate >> 2) & SPI_2XCLOCK_MASK);
}

