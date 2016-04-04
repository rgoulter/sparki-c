#include "Sparki.h"

// variables for communication between the IR read function and its interrupt
#define MAX_IR_PULSE 20000
volatile long timeSinceLastPulse = 0;
volatile long lastPulseTime = 0;
volatile uint16_t pulsesIR[50][2]; // LOW,HIGH
volatile uint8_t currentPulse = 0;
volatile uint8_t haltIRRead = 0;



// Uses timer3 to send on/off IR pulses according to the NEC IR transmission standard
// http://wiki.altium.com/display/ADOH/NEC+Infrared+Transmission+Protocol
// protocol. Turns off timer3 functions and timer4 motor/LED interference to avoid conflict
void sparki_sendIR(uint8_t code){
    char oldSREG = SREG;
    noInterrupts();  // Disable interrupts for 16 bit register access

    //***********************************************
    // Set up and tear down Timer3 and Timer4 roles
    //***********************************************

    // saves settings for timer3
    uint8_t TIMSK3_store = TIMSK3;
    uint8_t TCCR3A_store = TCCR3A;
    uint8_t TCCR3B_store = TCCR3B;
    uint8_t TCNT3_store = TCNT3;
    uint8_t EIMSK_store = EIMSK;

    uint8_t TIMSK4_store = TIMSK4;

    // wipe the timer settings
    TIMSK4 = 0;
    TIMSK3 = 0;
    TCCR3A = 0;
    TCCR3B = 0;
    TCNT3  = 0;
    EIMSK  = 0;

    TCCR3B |= _BV(CS31);      // set timer clock at 1/8th of CLK_i/o (=CLK_sys)
    OCR3B = 22;               // compare match register

    TIMSK3 |= (1 << OCIE3B);  // enable Timer3 compare interrupt B

    interrupts();  // re-enable interrupts
    SREG = oldSREG;


    //*****************************************
    // send the pulses
    //*****************************************


    // leadings 9ms pulse, 4.5ms gap
    irPulse(9000,4500);

    // 8 bit address
    for(int i=0; i<8; i++){
        irPulse(563,563); // NEC logical 0
    }

    // 8 bit address' logical inverse
    for(int i=0; i<8; i++){
        irPulse(563,1687); // NEC logical 1
    }

    // 8 bit command
    for(uint8_t i=0; i<8; i++){
        if( (code & (1<<i)) > 0 ){
            irPulse(563,1687); // NEC logical 1
        }
        else{
            irPulse(563,563);  // NEC logical 0
        }
    }

    // 8 bit command's logical inverse
    for(uint8_t i=0; i<8; i++){
        if( (code & (1<<i)) > 0 ){
            irPulse(563,563);  // NEC logical 0
        }
        else{
            irPulse(563,1687); // NEC logical 1
        }
    }

    // 562.5µs pulse to signal end of transmission
    irPulse(563,10); // NEC logical 1

    //*****************************************
    // restore Timer3 and Timer4 roles
    //*****************************************

    // restore the timer
    TIMSK4 = TIMSK4_store;
    TIMSK3 = TIMSK3_store;
    TCCR3A = TCCR3A_store;
    TCCR3B = TCCR3B_store;
    TCNT3  = TCNT3_store;
    EIMSK  = EIMSK_store;
}

void sparki_irPulse(uint16_t on, uint16_t off){
    TIMSK3 |= (1 << OCIE3B);  // enable  38khz signal
    delayMicroseconds(on);
    TIMSK3 &= ~(1 << OCIE3B);  // disable 38khz signal
    PORTD &= ~(1<<7); // make sure the LED is off
    delayMicroseconds(off);
}

/*
   Returns the current IR Code.
   Uses the interrupt on INT6 (PE6, Pin 7) to do the signal reading
   If there is no code waiting, pass -1 back immediately.
   If there is a code, but its still reading, wait it out then return code
   Wipes the current stored code once read.

   NEC IR code details here:
http://wiki.altium.com/display/ADOH/NEC+Infrared+Transmission+Protocol
*/

int sparki_readIR(){
    uint8_t code = 0;
    if(currentPulse != 0){ // check there's a code waiting
        while( micros()-lastPulseTime <= MAX_IR_PULSE){
            delayMicroseconds(MAX_IR_PULSE);
        }; // wait for the reading to time out
        // tell the interrupt to not take any more codes
        haltIRRead = 1;

        // decode the signal
        for(int i=0; i<8; i++){
            if(pulsesIR[17+i][1] > 1000){
                code |= (1<<i);
            }
        }
        currentPulse = 0; // 'reset' the current IR pulse reading
        haltIRRead = 0;
        return (int)(code); // return the decoded value
    }
    else{
        return -1; // no signal found, return -1
    }
}

SIGNAL(INT6_vect) {
    if(haltIRRead != 1){
        long currentTime = micros(); // take the current time
        int pinStatus = PINE & (1 << 6); // read the current status of the IR Pin
        timeSinceLastPulse = currentTime-lastPulseTime;

        // Tell if its the start of the reading cycle or not (time since last pulse), starts low
        if( (timeSinceLastPulse >= MAX_IR_PULSE) && (pinStatus == LOW)){
            // if reading new pulse, set up. Wipes out the last pulse
            currentPulse = 0;
        }
        else{
            // otherwise, read the current code
            if(pinStatus){ //(PE6 high) pulse has risen from the end of a low pulse
                pulsesIR[currentPulse][0] = timeSinceLastPulse;
            }
            else{ //(PE6 low) pulse has fallen from the end of a high pulse
                pulsesIR[currentPulse][1] = timeSinceLastPulse;
                currentPulse++;
            }
        }
        lastPulseTime = currentTime;
    }
}

/*
 * private functions
 */

// set the number if steps for the given motor

ISR(TIMER3_COMPB_vect) // IR send function, operates at ~38khz when active
{
    PORTD ^= (1<<7); // toggle the IR LED pin
    TCNT3=0;
}

