#include <Sparki.h>  // include the robot library

void setup() {
  sparki_servo(SERVO_CENTER); // center the servo
}

// /------^-----\
// |            |
// | 69  70  71 |
// | 68  64  67 |
// |  7  21   9 |
// | 22  25  13 |
// | 12  24  94 |
// |  8  28  90 |
// | 66  82  74 |
// \____________/

int program = true;
void loop() {

  //Scan for IR Receiver
  int code = sparki_readIR();

   // if there is a valid remote button press
  if(code != -1){
    sparki_moveStop(); // stop the motor
    sparki_RGB(RGB_OFF); // clear the RGB
    program = false; // turn off the starter program
  }

  switch(code){
    case 70: sparki_moveForward(); break;
    case 21: sparki_moveBackward(); break;
    case 67:
    case 71: sparki_moveRight(); break;
    case 68:
    case 69: sparki_moveLeft(); break;
    case 64: sparki_moveStop();
             sparki_gripperStop();
             break;

    // Gripper Buttons
    case 9:  sparki_gripperOpen(); break;
    case 7:  sparki_gripperClose(); break;

    // buzzer
    case 74: sparki_beep(); break;

    // Servo
    case 90: sparki_servo(SERVO_LEFT); break;
    case 28: sparki_servo(SERVO_CENTER); break;
    case 8: sparki_servo(SERVO_RIGHT); break;

    // RGB LED
    case 25: sparki_RGB(RGB_OFF); break;
    case 12: sparki_RGB(RGB_RED); break;
    case 24: sparki_RGB(RGB_GREEN); break;
    case 94: sparki_RGB(RGB_BLUE); break;

    // Program Control
    case 66:
      sparki_moveStop();
      sparki_RGB(0,0,0);
      program = false; break;
    case 82:
      program = true; break;
  }

  // Run Autonomy Code if
  if(program == true){
    sparki_RGB(RGB_GREEN); // turn the light green
    sparki_moveForward(); // move Sparki forward
    int cm = sparki_ping(); // measures the distance with Sparki's eyes

    if(cm != -1) // make sure its not too close or too far
    {
        if(cm < 20) // if the distance measured is less than 10 centimeters
        {
            sparki_RGB(RGB_RED); // turn the light red
            sparki_beep(); // beep!
            sparki_moveBackward(10); // back up 10 centimeters
            sparki_moveRight(30); // rotate right 30 degrees
        }
    }
    delay(100); // wait 0.1 seconds (100 milliseconds)
  }
}

int main(int argc, char *argv[])
{
    init();
    sparki_begin();
    setup();
    while(1) { loop(); }
    return 0;
}
