#include "Sparki.h"
#include <limits.h>


const float STEPS_PER_CM             = STEPS_PER_REV/(WHEEL_DIAMETER_CM*PI);
//const float STEPS_PER_ROTATION     = (TRACK_WIDTH_CM / WHEEL_DIAMETER_CM) * STEPS_PER_REV ;  // robot rotation
//const float STEPS_PER_DEGREE       = STEPS_PER_ROTATION / 360.0;         // robot rotation
const float STEPS_PER_DEGREE         = (TRACK_WIDTH_CM / WHEEL_DIAMETER_CM) * STEPS_PER_REV / 360.0;
const float STEPS_PER_ARM_CM         = 650;


static int8_t step_dir[3];                 // -1 = ccw, 1 = cw

static uint8_t motor_speed[3];              // stores last set motor speed (0-100%)

static volatile uint8_t move_speed = 100;
static volatile uint8_t speed_index[3];
static volatile uint8_t speed_array[3][SPEED_ARRAY_LENGTH];
// for each motor, how many 200uS waits between each step.
// Cycles through an array of 10 of these counts to average
// for better speed control

static volatile int8_t step_index[3];       // index into _steps array
static uint8_t _steps_right[9];                   // bytes defining stepper coil activations
static uint8_t _steps_left[9];                   // bytes defining stepper coil activations
static volatile uint32_t remainingSteps[3]; // number of steps before stopping motor
static volatile uint32_t isRunning[3];      // tells if motor is running

static volatile int speedCounter[3];      // variable used in maintaing speed
static volatile int speedCount[3];      // what speedCount is set at when speed cycle resets



void begin_motor() {
    // defining steps for the stepper motors
    _steps_left[0] = 0x10;
    _steps_left[1] = 0x30;
    _steps_left[2] = 0x20;
    _steps_left[3] = 0x60;
    _steps_left[4] = 0x40;
    _steps_left[5] = 0xC0;
    _steps_left[6] = 0x80;
    _steps_left[7] = 0x90;
    _steps_left[8]  = 0x00;

    _steps_right[0] = 0x01;
    _steps_right[1] = 0x03;
    _steps_right[2] = 0x02;
    _steps_right[3] = 0x06;
    _steps_right[4] = 0x04;
    _steps_right[5] = 0x0C;
    _steps_right[6] = 0x08;
    _steps_right[7] = 0x09;
    _steps_right[8] = 0x00;

    // Setup initial Stepper settings
    motor_speed[MOTOR_LEFT] = motor_speed[MOTOR_RIGHT] = motor_speed[MOTOR_GRIPPER] = move_speed;

}


void isr_motor(volatile uint8_t *shift_outputs) {
    //   Determine what state the stepper coils are in
    for(byte motor=0; motor<3; motor++){
        if( remainingSteps[motor] > 1 ){ // check if finished stepping
            // speedCount determines the stepping frequency
            // interrupt speed (5khz) divided by speedCounter equals stepping freq
            // 1khz is the maximum reliable frequency at 5v, so we use 5 as the top speed
            // 5,000hz/5 = 1000hz = micro-stepping frequency
            if(speedCounter[motor] == 0) {
                step_index[motor] += step_dir[motor];
                remainingSteps[motor]--;
                speedCounter[motor] = speed_array[motor][speed_index[motor]];
                speed_index[motor]++;
                if(speed_index[motor] >= SPEED_ARRAY_LENGTH){
                    speed_index[motor] = 0;
                }
            }
            else{
                speedCounter[motor] = speedCounter[motor]-1;
            }

        }
        else {  // if this was the last step
            remainingSteps[motor] = 0;
            isRunning[motor] = false;
            step_index[motor] = 8;
            speedCounter[motor] = -1;
        }

        //   keep indicies from rolling over or under
        if( step_index[motor] >= 8){
            step_index[motor] = 0;
        }
        else if( step_index[motor] < 0){
            step_index[motor] = 7;
        }
        if(isRunning[motor] == false){
            step_index[motor] = 8;
        }
    }

    shift_outputs[0] |= _steps_right[step_index[MOTOR_RIGHT]];
    shift_outputs[0] |= _steps_left[step_index[MOTOR_GRIPPER]];
    shift_outputs[1] |= _steps_left[step_index[MOTOR_LEFT]];
}

/*
 * motor control (non-blocking, except when moving distances)
 * speed is percent 0-100
 */

void sparki_moveRight_deg(float deg)
{
    unsigned long steps = STEPS_PER_DEGREE*deg;
    if(deg == 0){
        sparki_moveRight();
    }
    else{
        if(deg < 0){
            sparki_moveLeft_deg(-deg);
        }
        else{
            sparki_stepRight(steps);
            while( sparki_areMotorsRunning() ){
                delay(1);
            }
        }
    }
}

void sparki_stepRight(unsigned long steps)
{
    sparki_motorRotate_steps(MOTOR_LEFT, DIR_CCW, move_speed, steps);
    sparki_motorRotate_steps(MOTOR_RIGHT, DIR_CCW, move_speed, steps);
}

void sparki_moveRight()
{
    sparki_motorRotate_steps(MOTOR_LEFT, DIR_CCW, move_speed, ULONG_MAX);
    sparki_motorRotate_steps(MOTOR_RIGHT, DIR_CCW, move_speed, ULONG_MAX);
}

void sparki_moveLeft_deg(float deg)
{
    unsigned long steps = STEPS_PER_DEGREE*deg;
    if(deg == 0){
        sparki_moveLeft();
    }
    else{
        if(deg < 0){
            sparki_moveRight_deg(-deg);
        }
        else{
            sparki_stepLeft(steps);
            while( sparki_areMotorsRunning() ){
                delay(1);
            }
        }
    }
}

void sparki_stepLeft(unsigned long steps)
{
    sparki_motorRotate_steps(MOTOR_LEFT,  DIR_CW, move_speed, steps);
    sparki_motorRotate_steps(MOTOR_RIGHT, DIR_CW, move_speed, steps);
}

void sparki_moveLeft()
{
    sparki_motorRotate_steps(MOTOR_LEFT,  DIR_CW, move_speed, ULONG_MAX);
    sparki_motorRotate_steps(MOTOR_RIGHT, DIR_CW, move_speed, ULONG_MAX);
}

void sparki_moveForward_cm(float cm)
{
    unsigned long steps = STEPS_PER_CM*cm;
    if(cm == 0){
        sparki_moveForward();
    }
    else{
        if(cm < 0){
            sparki_moveBackward_cm(-cm);
        }
        else{
            sparki_stepForward(steps);
            while( sparki_areMotorsRunning() ){
                delay(1);
            }
        }
    }
}

void sparki_stepForward(unsigned long steps)
{
    sparki_motorRotate_steps(MOTOR_LEFT, DIR_CCW, move_speed, steps);
    sparki_motorRotate_steps(MOTOR_RIGHT, DIR_CW, move_speed, steps);
}

void sparki_moveForward()
{
    sparki_motorRotate_steps(MOTOR_LEFT, DIR_CCW, move_speed, ULONG_MAX);
    sparki_motorRotate_steps(MOTOR_RIGHT, DIR_CW, move_speed, ULONG_MAX);
}

void sparki_moveBackward_cm(float cm)
{
    unsigned long steps = STEPS_PER_CM*cm;
    if(cm == 0){
        sparki_moveBackward();
    }
    else{
        if(cm < 0){
            sparki_moveForward_cm(-cm);
        }
        else{
            sparki_stepBackward(steps);
            while( sparki_areMotorsRunning() ){
                delay(1);
            }
        }
    }
}

void sparki_stepBackward(unsigned long steps)
{
    sparki_motorRotate_steps(MOTOR_LEFT,   DIR_CW, move_speed, steps);
    sparki_motorRotate_steps(MOTOR_RIGHT, DIR_CCW, move_speed, steps);
}

void sparki_moveBackward()
{
    sparki_motorRotate_steps(MOTOR_LEFT,   DIR_CW, move_speed, ULONG_MAX);
    sparki_motorRotate_steps(MOTOR_RIGHT, DIR_CCW, move_speed, ULONG_MAX);
}

void sparki_moveStop()
{
    sparki_motorStop(MOTOR_LEFT);
    sparki_motorStop(MOTOR_RIGHT);
}

void sparki_gripperOpen()
{
    sparki_motorRotate_steps(MOTOR_GRIPPER, DIR_CCW, move_speed, ULONG_MAX);
}
void sparki_gripperOpen_cm(float cm)
{
    sparki_motorRotate_steps(MOTOR_GRIPPER, DIR_CCW, move_speed, (unsigned long)(cm*STEPS_PER_ARM_CM));
}

void sparki_gripperClose()
{
    sparki_motorRotate_steps(MOTOR_GRIPPER, DIR_CW, move_speed, ULONG_MAX);
}
void sparki_gripperClose_cm(float cm)
{
    sparki_motorRotate_steps(MOTOR_GRIPPER, DIR_CW, move_speed, (unsigned long)(cm*STEPS_PER_ARM_CM));
}

void sparki_gripperStop()
{
    sparki_motorStop(MOTOR_GRIPPER);
}

void sparki_speed(uint8_t speed)
{
    move_speed = speed;
}

void sparki_motorRotate(int motor, int direction, int speed)
{
    sparki_motorRotate_steps(motor, direction, speed, ULONG_MAX);
}

void sparki_motorRotate_steps(int motor, int direction, int speed, long steps)
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
        // The magic value here was 500,
        // as per the remark in the ISR, 500 = 5 * max_speed; because
        // of the max reliable frequency of stepper motors.
        //
        // Maybe something is off in the timing here, but motors don't work
        // for that magic val; using 800 instead. (750 not quite high enough).
        int base_waits = 800/speed;
        int remainder_waits = (int)((800.0/(float)(speed) - (float)(base_waits))*SPEED_ARRAY_LENGTH);

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
    sparki_motorRotate_steps(motor, 1, 0, 0);
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
