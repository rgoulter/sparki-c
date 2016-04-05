#include <Sparki.h> // include the sparki library

void setup()
{
}

void loop() {
  int left   = sparki_lightLeft();   // measure the left light sensor
  int center = sparki_lightCenter(); // measure the center light sensor
  int right  = sparki_lightRight();  // measure the right light sensor

  if ( (center > left) && (center > right) ) // if the center light is the strongest
  {
    sparki_moveForward(); // move forward
  }

  if ( (left > center) && (left > right) )  // if the left light is the strongest
  {
    sparki_moveLeft(); // turn left
  }

  if ( (right > center) && (right > left) )  // if the right light is the strongest
  {
    sparki_moveRight(); // turn right
  }

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
