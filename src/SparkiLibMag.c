#ifndef NO_MAG
float sparki_readMag(){
    WireRead(DataRegisterBegin, RawMagDataLength);
    sparki_xAxisMag = ((mag_buffer[0] << 8) | mag_buffer[1]) * M_SCALE;
    sparki_zAxisMag = ((mag_buffer[2] << 8) | mag_buffer[3]) * M_SCALE;
    sparki_yAxisMag = ((mag_buffer[4] << 8) | mag_buffer[5]) * M_SCALE;
}

float sparki_compass(){
    readMag();

    float heading = atan2(sparki_yAxisMag, sparki_xAxisMag);

    if(heading < 0)
        heading += 2*PI;
    if(heading > 2*PI)
        heading -= 2*PI;

    float headingDegrees = heading * 180/M_PI;
    return headingDegrees;
}

float sparki_magX(){
    readMag();
    return sparki_xAxisMag;
}

float sparki_magY(){
    readMag();
    return sparki_yAxisMag;
}

float sparki_magZ(){
    readMag();
    return sparki_zAxisMag;
}

void sparki_WireWrite(int address, int data){
    Wire.beginTransmission(HMC5883L_Address);
    Wire.write(address);
    Wire.write(data);
    Wire.endTransmission();
}

uint8_t* sparki_WireRead(int address, int length){
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
