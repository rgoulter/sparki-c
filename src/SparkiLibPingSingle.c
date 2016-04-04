int sparki_ping_single(){
    long duration;
    float cm;
    digitalWrite(ULTRASONIC_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(ULTRASONIC_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRASONIC_TRIG, LOW);


    uint8_t bit = digitalPinToBitMask(ULTRASONIC_ECHO);
    uint8_t port = digitalPinToPort(ULTRASONIC_ECHO);
    uint8_t stateMask = (HIGH ? bit : 0);

    unsigned long startCount = 0;
    unsigned long endCount = 0;
    unsigned long width = 0; // keep initialization out of time critical area

    // convert the timeout from microseconds to a number of times through
    // the initial loop; it takes 16 clock cycles per iteration.
    unsigned long numloops = 0;
    unsigned long maxloops = 5000;

    // wait for any previous pulse to end
    while ((*portInputRegister(port) & bit) == stateMask)
        if (numloops++ == maxloops)
            return -1;

    // wait for the pulse to start
    while ((*portInputRegister(port) & bit) != stateMask)
        if (numloops++ == maxloops)
            return -1;

    startCount = micros();
    // wait for the pulse to stop
    while ((*portInputRegister(port) & bit) == stateMask) {
        if (numloops++ == maxloops)
            return -1;
        delayMicroseconds(10); //loop 'jams' without this
        if((micros() - startCount) > 58000 ){ // 58000 = 1000CM
            return -1;
            break;
        }
    }
    duration = micros() - startCount;
    //--------- end pulsein
    cm = (float)duration / 29.0 / 2.0;
    return int(cm);
}

int sparki_ping(){
    int attempts = 5;
    float distances [attempts];
    for(int i=0; i<attempts; i++){
        distances[i] = ping_single();
        delay(20);
    }

    // sort them in order
    int i, j;
    float temp;

    for (i = (attempts - 1); i > 0; i--)
    {
        for (j = 1; j <= i; j++)
        {
            if (distances[j-1] > distances[j])
            {
                temp = distances[j-1];
                distances[j-1] = distances[j];
                distances[j] = temp;
            }
        }
    }

    // return the middle entry
    return int(distances[(int)ceil((float)attempts/2.0)]);
}
