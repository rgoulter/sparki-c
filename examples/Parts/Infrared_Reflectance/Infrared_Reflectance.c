/*******************************************
  IR Sensors

  Sparki has sensors underneath that
  sense objects in the way, and differences
  in the surface color. This example shows
  the numbers from those on the LCD. Try
  seeing what surfaces affect the numbers.

  http://arcbotics.com/products/sparki/parts/infrared-reflectance-sensor/
********************************************/
#include "Sparki.h"

void setup()
{
}

void loop() {
  sparki_clearLCD(); // wipe the screen

  int edgeLeft   = sparki_edgeLeft();   // measure the left edge IR sensor
  int lineLeft   = sparki_lineLeft();   // measure the left IR sensor
  int lineCenter = sparki_lineCenter(); // measure the center IR sensor
  int lineRight  = sparki_lineRight();  // measure the right IR sensor
  int edgeRight  = sparki_edgeRight();  // measure the right edge IR sensor

  // write the measurements to the screen
  sparki_textWrite_sz("Edge Left:   ");
  sparki_textWrite_i(edgeLeft);
  sparki_textWriteln();

  sparki_textWriteln();

  sparki_textWrite_sz("Line Left:   ");
  sparki_textWrite_i(lineLeft);
  sparki_textWriteln();

  sparki_textWrite_sz("Line Center: ");
  sparki_textWrite_i(lineCenter);
  sparki_textWriteln();

  sparki_textWrite_sz("Line Right:  ");
  sparki_textWrite_i(lineRight);
  sparki_textWriteln();

  sparki_textWriteln();

  sparki_textWrite_sz("Edge Right:  ");
  sparki_textWrite_i(edgeRight);
  sparki_textWriteln();

  sparki_updateLCD(); // display all of the information written to the screen
  delay(100);
}

int main(int argc, char **argv) {
  init();
  sparki_begin();
  setup();
  while(1) { loop(); }
  return 0;
}
