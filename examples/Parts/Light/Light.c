/*******************************************
 Basic Light Sensor test

 Sparki has 3 light sensors in the front,
 one each on the left, middle and right.
 This program shows how to read the sensors
 and display the information on the LCD.

 http://arcbotics.com/light-sensor/
********************************************/
#include "Sparki.h" // include the sparki library

void setup()
{
}

void loop() {
  sparki_clearLCD(); // wipe the screen

  int left   = sparki_lightLeft();   // measure the left light sensor
  int center = sparki_lightCenter(); // measure the center light sensor
  int right  = sparki_lightRight();  // measure the right light sensor

  // write the measurements to the screen
  sparki_textWrite_sz("Left   ");
  sparki_textWrite_i(left);
  sparki_textWriteln();

  sparki_textWrite_sz("Center ");
  sparki_textWrite_i(center);
  sparki_textWriteln();

  sparki_textWrite_sz("Right  ");
  sparki_textWrite_i(right);
  sparki_textWriteln();

  sparki_updateLCD(); // display all of the information written to the screen
  delay(100);
}

int main(int argc, char *argv[]) {
    init();
    sparki_begin();
    setup();
    while(1) { loop(); }
    return 0;
}
