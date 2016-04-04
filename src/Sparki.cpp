#include "Sparki.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
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

static int8_t step_dir[3];                 // -1 = ccw, 1 = cw

static uint8_t motor_speed[3];              // stores last set motor speed (0-100%)

uint8_t pixel_color = WHITE;

uint8_t ir_active = 1;

uint8_t lcd_initialized = 0; // keeps track of whether LCD buffer has been loaded into memory or not

uint8_t LCD_TYPE = 0; // 0=small, 1=large

static volatile uint8_t move_speed = 100;
static volatile uint8_t speed_index[3];
static volatile uint8_t speed_array[3][SPEED_ARRAY_LENGTH];
// for each motor, how many 200uS waits between each step.
// Cycles through an array of 10 of these counts to average
// for better speed control

static volatile int8_t step_index[3];       // index into _steps array
static uint8_t _steps_right[9];                   // bytes defining stepper coil activations
static uint8_t _steps_left[9];                   // bytes defining stepper coil activations
static volatile uint32_t remainingSteps[3]; // number of steps before stopping motor
static volatile uint32_t isRunning[3];      // tells if motor is running

static volatile int speedCounter[3];      // variable used in maintaing speed
static volatile int speedCount[3];      // what speedCount is set at when speed cycle resets

static volatile uint8_t shift_outputs[3];      // tells if motor is running

// variables for communication between the IR read function and its interrupt
#define MAX_IR_PULSE 20000
volatile long timeSinceLastPulse = 0;
volatile long lastPulseTime = 0;
volatile uint16_t pulsesIR[50][2]; // LOW,HIGH
volatile uint8_t currentPulse = 0;
volatile uint8_t haltIRRead = 0;


// values for the servo
volatile int8_t servo_deg_offset = 0;

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

    if( eeprom_read_byte((unsigned char *) 0) > 127) {
        servo_deg_offset = -256+eeprom_read_byte((unsigned char *) 0);
    }
    else{
        servo_deg_offset = eeprom_read_byte((unsigned char *) 0);
    }

    if( eeprom_read_byte((unsigned char *) 1) == 88) {
        LCD_TYPE = 1; // large LCD
    }
    else{
        LCD_TYPE = 0; // small LCD
    }

    // keep offset from going too off if EEPROM corrupted
    if (servo_deg_offset > MAX_SERVO_OFFSET){
        servo_deg_offset = 0;
    }
    if (servo_deg_offset < -MAX_SERVO_OFFSET){
        servo_deg_offset = 0;
    }

    //servo(SERVO_CENTER);


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

    // defining steps for the stepper motors
    _steps_left[0] = 0x10;
    _steps_left[1] = 0x30;
    _steps_left[2] = 0x20;
    _steps_left[3] = 0x60;
    _steps_left[4] = 0x40;
    _steps_left[5] = 0xC0;
    _steps_left[6] = 0x80;
    _steps_left[7] = 0x90;
    _steps_left[8]  = 0x00;

    _steps_right[0] = 0x01;
    _steps_right[1] = 0x03;
    _steps_right[2] = 0x02;
    _steps_right[3] = 0x06;
    _steps_right[4] = 0x04;
    _steps_right[5] = 0x0C;
    _steps_right[6] = 0x08;
    _steps_right[7] = 0x09;
    _steps_right[8] = 0x00;

#ifdef NOLCD
    progmem_lcd_logo();
#else
    beginDisplay();
    updateLCD();
#endif



    // Setup initial Stepper settings
    motor_speed[MOTOR_LEFT] = motor_speed[MOTOR_RIGHT] = motor_speed[MOTOR_GRIPPER] = move_speed;

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

    // IR Detection Switch
    if(irSwitch == 0){
        shift_outputs[1] &= 0xF7;
    }
    else{
        shift_outputs[1] |= 0x08;
    }

    //// Motor Control ////
    //   Determine what state the stepper coils are in
    for(byte motor=0; motor<3; motor++){
        if( remainingSteps[motor] > 1 ){ // check if finished stepping
            // speedCount determines the stepping frequency
            // interrupt speed (5khz) divided by speedCounter equals stepping freq
            // 1khz is the maximum reliable frequency at 5v, so we use 5 as the top speed
            // 5,000hz/5 = 1000hz = micro-stepping frequency
            if(speedCounter[motor] == 0) {
                step_index[motor] += step_dir[motor];
                remainingSteps[motor]--;
                speedCounter[motor] = speed_array[motor][speed_index[motor]];
                speed_index[motor]++;
                if(speed_index[motor] >= SPEED_ARRAY_LENGTH){
                    speed_index[motor] = 0;
                }
            }
            else{
                speedCounter[motor] = speedCounter[motor]-1;
            }

        }
        else {  // if this was the last step
            remainingSteps[motor] = 0;
            isRunning[motor] = false;
            step_index[motor] = 8;
            speedCounter[motor] = -1;
        }

        //   keep indicies from rolling over or under
        if( step_index[motor] >= 8){
            step_index[motor] = 0;
        }
        else if( step_index[motor] < 0){
            step_index[motor] = 7;
        }
        if(isRunning[motor] == false){
            step_index[motor] = 8;
        }
    }

    shift_outputs[0] |= _steps_right[step_index[MOTOR_RIGHT]];
    shift_outputs[0] |= _steps_left[step_index[MOTOR_GRIPPER]];
    shift_outputs[1] |= _steps_left[step_index[MOTOR_LEFT]];

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

