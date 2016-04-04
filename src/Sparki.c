#include "Sparki.h"
//#include <inttypes.h>
#include <stdio.h>
#include <string.h>
//#include <limits.h>
//#include <Arduino.h>

#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include "CSPI.h"


#ifndef NO_MAG
#include "SparkiWire.h"
// variables for the magnetometer
volatile uint8_t mag_buffer[RawMagDataLength];
#endif


uint8_t ir_active = 1;

uint8_t lcd_initialized = 0; // keeps track of whether LCD buffer has been loaded into memory or not

uint8_t LCD_TYPE = 0; // 0=small, 1=large

static volatile uint8_t shift_outputs[3];      // tells if motor is running



//R These should be in the header at some point.
void begin_servo(void);
void begin_motor(void);
void isr_motor(int *);
void isr_irreflectance(int *);

// SparkiClass sparki;

void sparki_begin( ) {

    // Serial.begin(9600);  //R XXX Serial is C++ Arduino
    // Serial1.begin(9600); //R XXX Let's not use, temporarily.

    // set up the Status LED
    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, LOW);

    // Setup IR Send
    pinMode(IR_SEND, OUTPUT);

    // Setup Ultrasonic
    pinMode(ULTRASONIC_TRIG, OUTPUT);
    pinMode(ULTRASONIC_ECHO, INPUT);

    // Setup Servo
    pinMode(SERVO, OUTPUT);
    //startServoTimer();

    if( eeprom_read_byte((unsigned char *) 1) == 88) {
        LCD_TYPE = 1; // large LCD
    }
    else{
        LCD_TYPE = 0; // small LCD
    }

    begin_servo();


    // Setup the SPI bus for the shift register
    // !!! Need to remove the essential functions from the SPI Library,
    // !!! and include in the main code
    SPI_begin();
    SPI_setClockDivider(SPI_CLOCK_DIV2);

    // Set the shift-register clock select pin to output
    DDRD |= (1<<5);

    // Clear out the shift registers
    PORTD &= 0xDF;    // pull PD5 low
    SPI_transfer(shift_outputs[1]);
    SPI_transfer(shift_outputs[0]);
    PORTD |= 0x20;    // pull PD5 high to latch in spi transfers

    begin_beep();
    begin_irreflectance();
    begin_motor();


#ifdef NOLCD
    progmem_lcd_logo();
#else
    beginDisplay();
    updateLCD();
#endif


    // Set up the scheduler routine to run every 200uS, based off Timer4 interrupt
    cli();          // disable all interrupts
    TCCR4A = 0;
    TCCR4B = 0;
    TCNT4  = 0;

    OCR4A = 48;               // compare match register 64MHz/2048 = 31250hz
    //TCCR4B |= (1 << WGM12);   // CTC mode
    TCCR4B = 0x06;
    //TCCR4B = _BV(CS43) | _BV(CS42);            // CLK/2048 prescaler
    TIMSK4 |= (1 << OCIE4A);  // enable Timer4 compare interrupt A
    sei();             // enable all interrupts

    // Setup the IR Remote Control pin and pin interrupt
    noInterrupts();
    pinMode(IR_RECEIVE, INPUT);

    // Setup the pin interrupt for INT6 (Pin 7) to trigger the IR function
    EICRB = (EICRB & ~((1 << ISC60) | (1 << ISC61))) | (CHANGE << ISC60);
    EIMSK |= (1 << INT6);

    interrupts();
#ifndef NO_ACCEL
    i2cInit(); // start up i2c for the accelerometer (not mag)
    initAccelerometer();
#endif

#ifndef NO_MAG
    WireWrite(ConfigurationRegisterB, (0x01 << 5));
    WireWrite(ModeRegister, Measurement_Continuous);
    readMag(); // warm it up
#endif

}

float sparki_systemVoltage(){
    float voltage=0;

    pinMode(BATTERY_MONITOR, INPUT);
    for(uint8_t i=0; i<10; i++){
        voltage += analogRead(10);
        delay(1);
    }
    pinMode(BATTERY_MONITOR, OUTPUT);

    voltage = voltage/10.0;
    voltage = voltage*0.00868; // convert reading to voltage

    return voltage;
}

/***********************************************************************************
  The Scheduler
  Every 200uS (5,000 times a second), we update the 2 shift registers used to increase
  the amount of outputs the processor has
 ***********************************************************************************/

static volatile uint8_t shift_old_0 = 0x00;
static volatile uint8_t shift_old_1 = 0x00;

ISR(TIMER4_COMPA_vect)          // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
    //void sparki_scheduler(){
    // Clear the timer interrupt counter
    TCNT4=0;

    shift_old_0 = shift_outputs[0];
    shift_old_1 = shift_outputs[1];

    // clear the shift register values so we can re-write them
    shift_outputs[0] = 0x00;
    shift_outputs[1] = 0x00;

    // Update the RGB leds
    isr_RGB();

    //R *** IR Reflectance ***
    // IR Detection Switch
    isr_irreflectance(&(shift_outputs[1]));

    //// Motor Control ////
    isr_motor(shift_outputs);

    if( (shift_old_0 != shift_outputs[0]) || (shift_old_1 != shift_outputs[1]) ){
        //return LCD chip select high
        //digitalWrite(LCD_CS,HIGH);
        PORTB |= 0x01;

        //output values to shift registers
        PORTD &= ~(1<<5);    // pull PD5 (shift-register latch) low
        SPI_transfer(shift_outputs[1]);
        SPI_transfer(shift_outputs[0]);
        PORTD |= (1<<5);    // pull PD5 (shift-register latch) high
    }
}

