#include "Sparki.h"

// values for the servo
volatile int8_t servo_deg_offset = 0;


void begin_servo() {
    if( eeprom_read_byte((unsigned char *) 0) > 127) {
        servo_deg_offset = -256+eeprom_read_byte((unsigned char *) 0);
    }
    else{
        servo_deg_offset = eeprom_read_byte((unsigned char *) 0);
    }

    // keep offset from going too off if EEPROM corrupted
    if (servo_deg_offset > MAX_SERVO_OFFSET){
        servo_deg_offset = 0;
    }
    if (servo_deg_offset < -MAX_SERVO_OFFSET){
        servo_deg_offset = 0;
    }

    //servo(SERVO_CENTER);
}

void sparki_startServoTimer(){
    char oldSREG = SREG;
    noInterrupts();                                       // Disable interrupts for 16 bit register access
    TCCR1A = 0;                                           // clear control register A
    TCCR1B = _BV(WGM13);                                  // set mode 8: phase and frequency correct pwm, stop the timer
    ICR1 = 20000;                                         // ICR1 is TOP in p & f correct pwm mode
    TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
    TCCR1B |= 0x02;                                       // reset clock select register, and starts the clock
    DDRB |= _BV(PORTB1);                                  // sets data direction register for pwm output pin
    TCCR1A |= _BV(COM1A1);                                // activates the output pin
    interrupts();                                         // re-enable interrupts
    SREG = oldSREG;
}

void sparki_servo(int deg)
{
    sparki_startServoTimer();
    int duty = (int)(((((float)(-deg+servo_deg_offset)*2000/180)+1500)/20000)*1024); // compute the duty cycle for the servo
    //0 = 26
    //180 = 128

    unsigned long dutyCycle = 20000;
    dutyCycle *= duty;
    dutyCycle >>= 10;

    char oldSREG = SREG;
    noInterrupts();
    OCR1A = dutyCycle;

    SREG = oldSREG;
    interrupts();
}
