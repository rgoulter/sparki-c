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

void SparkiClass::setMux(uint8_t A, uint8_t B, uint8_t C){
    digitalWrite(MUX_A, A);
    digitalWrite(MUX_B, B);
    digitalWrite(MUX_C, C);
    delay(1);
}

/*
 * Light Sensors
 */

int SparkiClass::lightRight(){
    setMux(LIGHT_RIGHT);
    return analogRead(MUX_ANALOG);
}

int SparkiClass::lightCenter(){
    setMux(LIGHT_CENTER);
    return analogRead(MUX_ANALOG);
}

int SparkiClass::lightLeft(){
    setMux(LIGHT_LEFT);
    return analogRead(MUX_ANALOG);
}

/*
 * Infrared line sensors
 */


int SparkiClass::edgeRight(){
    setMux(IR_EDGE_RIGHT);
    return readSensorIR(MUX_ANALOG);
}

int SparkiClass::lineRight(){
    setMux(IR_LINE_RIGHT);
    return readSensorIR(MUX_ANALOG);
}

int SparkiClass::lineCenter(){
    setMux(IR_LINE_CENTER);
    return readSensorIR(MUX_ANALOG);
}

int SparkiClass::lineLeft(){
    setMux(IR_LINE_LEFT);
    return readSensorIR(MUX_ANALOG);
}

int SparkiClass::edgeLeft(){
    setMux(IR_EDGE_LEFT);
    return readSensorIR(MUX_ANALOG);
}

int SparkiClass::readSensorIR(int pin){
    int read = 0;
    onIR();
    read = analogRead(pin);
    offIR();
    return read;
}

void SparkiClass::onIR()  // turns off the IR Detection LEDs
{
    irSwitch = 1;
    delay(1); // give time for a scheduler cycle to run
}

void SparkiClass::offIR() // turns off the IR Detection LEDs
{
    irSwitch = 0;
    delay(1); // give time for a scheduler cycle to run
}

int SparkiClass::readBlindSensorIR(int pin0, int pin1, int pin2){
    int read = 0;
    setMux(pin0, pin1, pin2);
    delay(1);
    read = analogRead(MUX_ANALOG);
    delay(1);
    return read;
}

int SparkiClass::diffIR(int pin0, int pin1, int pin2){
    setMux(pin0, pin1, pin2);
    delay(1);
    int readOff = analogRead(MUX_ANALOG);
    delay(10);
    onIR();
    int readOn = analogRead(MUX_ANALOG);
    offIR();
    return readOff-readOn;
}
