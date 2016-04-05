#include <Sparki.h> // include the sparki library

void setup()
{

}

void loop()
{
    float x = sparki_accelX();
    float y = sparki_accelY();

    // is Sparki already facing downhill or uphill?
    if ( (x < 1) && (x > -1) )
    {
      if ( y < 0 ) // if sparki is facing uphill
      {
        sparki_moveForward();
      }
      if ( y > 0 ) // if sparki is facing downhill
      {
        sparki_moveBackward();
      }
    }
    else{ // if Sparki is not facing uphill or downhill, have Sparki turn until it is
      if ( x > 1 ) // if tilted right
      {
        sparki_moveLeft();
      }
      if ( x < -1 ) // if tilted left
      {
        sparki_moveRight();
      }
    }
    delay(100);
}

int main(int argc, char *argv[])
{
    init();
    sparki_begin();
    setup();
    while(1) { loop(); }
    return 0;
}
