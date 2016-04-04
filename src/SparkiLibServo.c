void SparkiClass::startServoTimer(){
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

void SparkiClass::servo(int deg)
{
    startServoTimer();
    int duty = int((((float(-deg+servo_deg_offset)*2000/180)+1500)/20000)*1024); // compute the duty cycle for the servo
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
