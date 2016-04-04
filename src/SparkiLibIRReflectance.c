#include "Sparki.h"



static volatile uint8_t irSwitch;
// static volatile uint8_t irSwitch2;


void begin_irreflectance() {
    // Setup Analog Multiplexer
    pinMode(MUX_ANALOG, INPUT);
    pinMode(MUX_A, OUTPUT);
    pinMode(MUX_B, OUTPUT);
    pinMode(MUX_C, OUTPUT);

    // Setup the IR Switch
    irSwitch = 0;
}

// shift_output1 for the shift_outputs[1]
void isr_irreflectance(volatile uint8_t *shift_output1) {
    if(irSwitch == 0){
        *shift_output1 &= 0xF7;
    }
    else{
        *shift_output1 |= 0x08;
    }
}

void sparki_setMux(uint8_t A, uint8_t B, uint8_t C){
    digitalWrite(MUX_A, A);
    digitalWrite(MUX_B, B);
    digitalWrite(MUX_C, C);
    delay(1);
}

/*
 * Light Sensors
 */

int sparki_lightRight(){
    sparki_setMux(LIGHT_RIGHT);
    return analogRead(MUX_ANALOG);
}

int sparki_lightCenter(){
    sparki_setMux(LIGHT_CENTER);
    return analogRead(MUX_ANALOG);
}

int sparki_lightLeft(){
    sparki_setMux(LIGHT_LEFT);
    return analogRead(MUX_ANALOG);
}

/*
 * Infrared line sensors
 */


int sparki_edgeRight(){
    sparki_setMux(IR_EDGE_RIGHT);
    return sparki_readSensorIR(MUX_ANALOG);
}

int sparki_lineRight(){
    sparki_setMux(IR_LINE_RIGHT);
    return sparki_readSensorIR(MUX_ANALOG);
}

int sparki_lineCenter(){
    sparki_setMux(IR_LINE_CENTER);
    return sparki_readSensorIR(MUX_ANALOG);
}

int sparki_lineLeft(){
    sparki_setMux(IR_LINE_LEFT);
    return sparki_readSensorIR(MUX_ANALOG);
}

int sparki_edgeLeft(){
    sparki_setMux(IR_EDGE_LEFT);
    return sparki_readSensorIR(MUX_ANALOG);
}

int sparki_readSensorIR(int pin){
    int read = 0;
    onIR();
    read = analogRead(pin);
    offIR();
    return read;
}

void sparki_onIR()  // turns off the IR Detection LEDs
{
    irSwitch = 1;
    delay(1); // give time for a scheduler cycle to run
}

void sparki_offIR() // turns off the IR Detection LEDs
{
    irSwitch = 0;
    delay(1); // give time for a scheduler cycle to run
}

int sparki_readBlindSensorIR(int pin0, int pin1, int pin2){
    int read = 0;
    sparki_setMux(pin0, pin1, pin2);
    delay(1);
    read = analogRead(MUX_ANALOG);
    delay(1);
    return read;
}

int sparki_diffIR(int pin0, int pin1, int pin2){
    sparki_setMux(pin0, pin1, pin2);
    delay(1);
    int readOff = analogRead(MUX_ANALOG);
    delay(10);
    onIR();
    int readOn = analogRead(MUX_ANALOG);
    offIR();
    return readOff-readOn;
}
