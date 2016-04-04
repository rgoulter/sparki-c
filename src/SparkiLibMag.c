#ifndef NO_MAG
float SparkiClass::readMag(){
    WireRead(DataRegisterBegin, RawMagDataLength);
    xAxisMag = ((mag_buffer[0] << 8) | mag_buffer[1]) * M_SCALE;
    zAxisMag = ((mag_buffer[2] << 8) | mag_buffer[3]) * M_SCALE;
    yAxisMag = ((mag_buffer[4] << 8) | mag_buffer[5]) * M_SCALE;
}

float SparkiClass::compass(){
    readMag();

    float heading = atan2(yAxisMag,xAxisMag);

    if(heading < 0)
        heading += 2*PI;
    if(heading > 2*PI)
        heading -= 2*PI;

    float headingDegrees = heading * 180/M_PI;
    return headingDegrees;
}

float SparkiClass::magX(){
    readMag();
    return xAxisMag;
}

float SparkiClass::magY(){
    readMag();
    return yAxisMag;
}

float SparkiClass::magZ(){
    readMag();
    return zAxisMag;
}

void SparkiClass::WireWrite(int address, int data){
    Wire.beginTransmission(HMC5883L_Address);
    Wire.write(address);
    Wire.write(data);
    Wire.endTransmission();
}

uint8_t* SparkiClass::WireRead(int address, int length){
    Wire.beginTransmission(HMC5883L_Address);
    Wire.write(DataRegisterBegin);
    Wire.endTransmission();

    Wire.beginTransmission(HMC5883L_Address);
    Wire.requestFrom(HMC5883L_Address, RawMagDataLength);

    if(Wire.available() == RawMagDataLength)
    {
        for(uint8_t i = 0; i < RawMagDataLength; i++)
        {
            mag_buffer[i] = Wire.read();
        }
    }
    Wire.endTransmission();
}
#endif
