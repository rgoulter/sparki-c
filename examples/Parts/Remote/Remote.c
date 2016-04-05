/*******************************************
  IR Remote

  Sparki has a sensor that lets it receives
  commands from the included remote control.
  Try moving it around!

  http://arcbotics.com/infrared-remote-receiver/
********************************************/
#include <Sparki.h> // include the sparki library

void setup()
{
  sparki_clearLCD();
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

void loop()
{
  int code = sparki_readIR();

  if(code != -1){
    sparki_textWrite_sz("Received code: ");
    sparki_textWrite_i(code);
    sparki_textWriteln();
  }

  switch(code){

  // Movement buttons
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

  default:
    break;
  }

  sparki_updateLCD();
}

int main(int argc, char *argv[])
{
    init();
    sparki_begin();
    setup();
    while(1) { loop(); }
    return 0;
}
