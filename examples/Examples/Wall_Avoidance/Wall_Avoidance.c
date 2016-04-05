#include <Sparki.h> // include the sparki library

void setup() {
  sparki_servo(SERVO_CENTER); // Center the Servo
}

void loop()
{
    sparki_RGB(RGB_GREEN); // turn the light green
    sparki_moveForward(); // move Sparki forward
    int cm = sparki_ping(); // measures the distance with Sparki's eyes

    if(cm != -1) // make sure its not too close or too far
    {
        if(cm < 20) // if the distance measured is less than 10 centimeters
        {
            sparki_RGB(RGB_RED); // turn the light red
            sparki_beep(); // beep!
            sparki_moveBackward_cm(10); // back up 10 centimeters
            sparki_moveRight_deg(30); // rotate right 30 degrees
        }
    }
    delay(100); // wait 0.1 seconds (100 milliseconds)
}

int main(int argc, char *argv[])
{
    init();
    sparki_begin();
    setup();
    while(1) { loop(); }
    return 0;
}
