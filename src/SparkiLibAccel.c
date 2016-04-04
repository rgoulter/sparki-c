/***********************************************************************************
  Accelerometer Library
 ***********************************************************************************/
#ifndef NO_ACCEL
#include "Sparkii2c.h"
// shares the values of the accelerometers
volatile float xAxisAccel;
volatile float yAxisAccel;
volatile float zAxisAccel;
#endif

#ifndef NO_ACCEL
void sparki_readAccelData()
{
    int accelCount[3];
    uint8_t rawData[6];  // x/y/z accel register data stored here


    readi2cRegisters(0x01, 6, &rawData[0], MMA8452_ADDRESS);  // Read the six raw data registers into data array


    // Loop to calculate 12-bit ADC and g value for each axis
    for (uint8_t i=0; i<6; i+=2)
    {
        accelCount[i/2] = ((rawData[i] << 8) | rawData[i+1]) >> 4;  // Turn the MSB and LSB into a 12-bit value
        if (rawData[i] > 0x7F)
        {
            // If the number is negative, we have to make it so manually (no 12-bit data type)
            accelCount[i/2] = ~accelCount[i/2] + 1;
            accelCount[i/2] *= -1;  // Transform into negative 2's complement #
        }
    }
    xAxisAccel = (float) accelCount[0]/((1<<12)/(2*ACCEL_SCALE));
    yAxisAccel = (float) accelCount[1]/((1<<12)/(2*ACCEL_SCALE));
    zAxisAccel = (float) accelCount[2]/((1<<12)/(2*ACCEL_SCALE));
}

int sparki_initAccelerometer()
{
    uint8_t c = readi2cRegister(0x0D, MMA8452_ADDRESS);  // Read WHO_AM_I register
    if (c == 0x2A){ // WHO_AM_I should always be 0x2
        // Must be in standby to change registers, so we do that
        c = readi2cRegister(0x2A, MMA8452_ADDRESS);
        readi2cRegister(0x2A, c & ~(0x01), MMA8452_ADDRESS);

        // Set up the full scale range
        readi2cRegister(0x0E, ACCEL_SCALE >> 2, MMA8452_ADDRESS);

        // Setup the 3 data rate bits, from 0 to 7
        readi2cRegister(0x2A, readi2cRegister(0x2A, MMA8452_ADDRESS) & ~(0x38), MMA8452_ADDRESS);
        if (ACCEL_DATARATE <= 7)
            readi2cRegister(0x2A, readi2cRegister(0x2A, MMA8452_ADDRESS) | (ACCEL_DATARATE << 3), MMA8452_ADDRESS);

        // Set back to active mode to start reading
        c = readi2cRegister(0x2A, MMA8452_ADDRESS);
        readi2cRegister(0x2A, c | 0x01, MMA8452_ADDRESS);
        return 1;
    }
    else{
        return -1;
    }
}

float sparki_accelX(){
    readAccelData();
    return -xAxisAccel*9.8;
}
float sparki_accelY(){
    readAccelData();
    return -yAxisAccel*9.8;
}
float sparki_accelZ(){
    readAccelData();
    return -zAxisAccel*9.8;
}

// Read i registers sequentially, starting at address into the dest byte array
void sparki_readi2cRegisters(byte address, int i, byte * dest, uint8_t i2cAddress)
{
    i2cSendStart();
    i2cWaitForComplete();

    i2cSendByte((i2cAddress<<1)); // write 0xB4
    i2cWaitForComplete();

    i2cSendByte(address);  // write register address
    i2cWaitForComplete();

    i2cSendStart();
    i2cSendByte((i2cAddress<<1)|0x01); // write 0xB5
    i2cWaitForComplete();
    for (int j=0; j<i; j++)
    {
        i2cReceiveByte(-1); // -1 = True
        i2cWaitForComplete();
        dest[j] = i2cGetReceivedByte(); // Get MSB result
    }
    i2cWaitForComplete();
    i2cSendStop();

    cbi(TWCR, TWEN); // Disable TWI
    sbi(TWCR, TWEN); // Enable TWI
}

// Read a single byte from address and return it as a byte
byte sparki_readi2cRegister(uint8_t address, uint8_t i2cAddress)
{
    byte data;

    i2cSendStart();
    i2cWaitForComplete();

    i2cSendByte((i2cAddress<<1)); // Write 0xB4
    i2cWaitForComplete();

    i2cSendByte(address);  // Write register address
    i2cWaitForComplete();

    i2cSendStart();

    i2cSendByte((i2cAddress<<1)|0x01); // Write 0xB5
    i2cWaitForComplete();
    i2cReceiveByte(-1); // -1 = True
    i2cWaitForComplete();

    data = i2cGetReceivedByte();  // Get MSB result
    i2cWaitForComplete();
    i2cSendStop();

    cbi(TWCR, TWEN); // Disable TWI
    sbi(TWCR, TWEN); // Enable TWI

    return data;
}

// Writes a single byte (data) into address
void sparki_readi2cRegister(unsigned char address, unsigned char data, uint8_t i2cAddress)
{
    i2cSendStart();
    i2cWaitForComplete();

    i2cSendByte((i2cAddress<<1)); // Write 0xB4
    i2cWaitForComplete();

    i2cSendByte(address); // Write register address
    i2cWaitForComplete();

    i2cSendByte(data);
    i2cWaitForComplete();

    i2cSendStop();
}
#endif
