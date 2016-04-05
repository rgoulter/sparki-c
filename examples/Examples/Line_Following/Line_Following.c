/*******************************************
 Sparki Line-following example

 Threshold is the value that helps you
 determine what's black and white. Sparki's
 infrared reflectance sensors indicate white
 as close to 900, and black as around 200.
 This example uses a threshold of 700 for
 the example, but if you have a narrow line,
 or perhaps a lighter black, you may need to
 adjust.
********************************************/

#include <Sparki.h> // include the sparki library

void setup()
{
}

void loop() {
  int threshold = 700;

  int lineLeft   = sparki_lineLeft();   // measure the left IR sensor
  int lineCenter = sparki_lineCenter(); // measure the center IR sensor
  int lineRight  = sparki_lineRight();  // measure the right IR sensor

  if ( lineCenter < threshold ) // if line is below left line sensor
  {
    sparki_moveForward(); // move forward
  }
  else{
    if ( lineLeft < threshold ) // if line is below left line sensor
    {
      sparki_moveLeft(); // turn left
    }

    if ( lineRight < threshold ) // if line is below right line sensor
    {
      sparki_moveRight(); // turn right
    }
  }

  sparki_clearLCD(); // wipe the screen

  sparki_textWrite_sz("Line Left: "); // show left line sensor on screen
  sparki_textWrite_i(lineLeft);
  sparki_textWriteln();

  sparki_textWrite_sz("Line Center: "); // show center line sensor on screen
  sparki_textWrite_i(lineCenter);
  sparki_textWriteln();

  sparki_textWrite_sz("Line Right: "); // show right line sensor on screen
  sparki_textWrite_i(lineRight);
  sparki_textWriteln();

  sparki_updateLCD(); // display all of the information written to the screen

  delay(100); // wait 0.1 seconds
}

int main(int argc, char *argv[])
{
    init();
    sparki_begin();
    setup();
    while(1) { loop(); }
    return 0;
}

