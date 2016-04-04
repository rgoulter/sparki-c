/*
 * motor control (non-blocking, except when moving distances)
 * speed is percent 0-100
 */

void sparki_moveRight(float deg)
{
    unsigned long steps = STEPS_PER_DEGREE*deg;
    if(deg == 0){
        moveRight();
    }
    else{
        if(deg < 0){
            moveLeft(-deg);
        }
        else{
            stepRight(steps);
            while( areMotorsRunning() ){
                delay(1);
            }
        }
    }
}

void sparki_stepRight(unsigned long steps)
{
    motorRotate(MOTOR_LEFT, DIR_CCW, move_speed, steps);
    motorRotate(MOTOR_RIGHT, DIR_CCW, move_speed, steps);
}

void sparki_moveRight()
{
    motorRotate(MOTOR_LEFT, DIR_CCW, move_speed, ULONG_MAX);
    motorRotate(MOTOR_RIGHT, DIR_CCW, move_speed, ULONG_MAX);
}

void sparki_moveLeft(float deg)
{
    unsigned long steps = STEPS_PER_DEGREE*deg;
    if(deg == 0){
        moveLeft();
    }
    else{
        if(deg < 0){
            moveRight(-deg);
        }
        else{
            stepLeft(steps);
            while( areMotorsRunning() ){
                delay(1);
            }
        }
    }
}

void sparki_stepLeft(unsigned long steps)
{
    motorRotate(MOTOR_LEFT,  DIR_CW, move_speed, steps);
    motorRotate(MOTOR_RIGHT, DIR_CW, move_speed, steps);
}

void sparki_moveLeft()
{
    motorRotate(MOTOR_LEFT,  DIR_CW, move_speed, ULONG_MAX);
    motorRotate(MOTOR_RIGHT, DIR_CW, move_speed, ULONG_MAX);
}

void sparki_moveForward(float cm)
{
    unsigned long steps = STEPS_PER_CM*cm;
    if(cm == 0){
        moveForward();
    }
    else{
        if(cm < 0){
            moveBackward(-cm);
        }
        else{
            stepForward(steps);
            while( areMotorsRunning() ){
                delay(1);
            }
        }
    }
}

void sparki_stepForward(unsigned long steps)
{
    motorRotate(MOTOR_LEFT, DIR_CCW, move_speed, steps);
    motorRotate(MOTOR_RIGHT, DIR_CW, move_speed, steps);
}

void sparki_moveForward()
{
    motorRotate(MOTOR_LEFT, DIR_CCW, move_speed, ULONG_MAX);
    motorRotate(MOTOR_RIGHT, DIR_CW, move_speed, ULONG_MAX);
}

void sparki_moveBackward(float cm)
{
    unsigned long steps = STEPS_PER_CM*cm;
    if(cm == 0){
        moveBackward();
    }
    else{
        if(cm < 0){
            moveForward(-cm);
        }
        else{
            stepBackward(steps);
            while( areMotorsRunning() ){
                delay(1);
            }
        }
    }
}

void sparki_stepBackward(unsigned long steps)
{
    motorRotate(MOTOR_LEFT,   DIR_CW, move_speed, steps);
    motorRotate(MOTOR_RIGHT, DIR_CCW, move_speed, steps);
}

void sparki_moveBackward()
{
    motorRotate(MOTOR_LEFT,   DIR_CW, move_speed, ULONG_MAX);
    motorRotate(MOTOR_RIGHT, DIR_CCW, move_speed, ULONG_MAX);
}

void sparki_moveStop()
{
    motorStop(MOTOR_LEFT);
    motorStop(MOTOR_RIGHT);
}

void sparki_gripperOpen()
{
    motorRotate(MOTOR_GRIPPER, DIR_CCW, move_speed, ULONG_MAX);
}
void sparki_gripperOpen(float cm)
{
    motorRotate(MOTOR_GRIPPER, DIR_CCW, move_speed, (unsigned long)(cm*STEPS_PER_ARM_CM));
}

void sparki_gripperClose()
{
    motorRotate(MOTOR_GRIPPER, DIR_CW, move_speed, ULONG_MAX);
}
void sparki_gripperClose(float cm)
{
    motorRotate(MOTOR_GRIPPER, DIR_CW, move_speed, (unsigned long)(cm*STEPS_PER_ARM_CM));
}

void sparki_gripperStop()
{
    motorStop(MOTOR_GRIPPER);
}

void sparki_speed(uint8_t speed)
{
    move_speed = speed;
}

void sparki_motorRotate(int motor, int direction, int speed)
{
    motorRotate(motor, direction, speed, ULONG_MAX);
}

void sparki_motorRotate(int motor, int direction, int speed, long steps)
{
    //Serial.print("Motor ");Serial.print(motor); Serial.print(" rotate, dir= ");
    //Serial.print(direction); Serial.print(", steps= "); Serial.println(steps);

    motor_speed[motor] = speed; // speed in 1-100 precent

    // populate the speed array with multiples of 200us waits between steps
    // having 10 different waits allows finer grained control
    if(speed == 0){
        uint8_t oldSREG = SREG; cli();
        remainingSteps[motor] = 0;
        isRunning[motor] = false;
        SREG = oldSREG; sei();
    }
    else{
        int base_waits = 500/speed;
        int remainder_waits = int((500.0/float(speed) - float(base_waits))*SPEED_ARRAY_LENGTH);

        for(uint8_t i=0; i< (SPEED_ARRAY_LENGTH-remainder_waits); i++){
            speed_array[motor][i] = base_waits+1;
        }
        for(uint8_t i=(SPEED_ARRAY_LENGTH-remainder_waits); i<SPEED_ARRAY_LENGTH; i++){
            speed_array[motor][i] = base_waits;
        }

        uint8_t oldSREG = SREG; cli();
        speed_index[motor] = 0;
        speedCount[motor] = speed_array[motor][0];
        speedCounter[motor] = speedCount[motor];
        remainingSteps[motor] = steps;
        step_dir[motor] = direction;
        isRunning[motor] = true;
        SREG = oldSREG; sei();

        //Serial.print("base: ");
        //Serial.print(base_waits);
        //Serial.print(", remainder: ");
        //Serial.println(remainder_waits);
    }
    delay(1);
}

void sparki_motorStop(int motor)
{
    motorRotate(motor, 1, 0, 0);
}

// returns true if one or both motors a still stepping
bool sparki_areMotorsRunning()
{
    bool result;
    uint8_t oldSREG = SREG;
    cli();
    result =  isRunning[MOTOR_LEFT] || isRunning[MOTOR_RIGHT] || isRunning[MOTOR_GRIPPER] ;
    SREG = oldSREG;
    sei();
    return result;
}
