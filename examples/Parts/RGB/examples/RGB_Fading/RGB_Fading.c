/*******************************************
 RGB Fade

 Sparki has a Red, Green, Blue LED on its
 back. Using Red, Green and Blue, you can
 make any color you want. This program makes
 cycles through the 3 led by fading them.

 http://arcbotics.com/products/sparki/parts/rgb-led/
********************************************/
#include <Sparki.h> // include the sparki library

void setup()
{
}
int time = 3;
void loop()
{
  // make red brighter
  for(int i=0; i<100; i++){
    sparki_RGB(i,0,0);
    delay(time);
  }
  // make red dimmer
  for(int i=100; i>0; i--){
    sparki_RGB(i,0,0);
    delay(time);
  }

  // make green brighter
  for(int i=0; i<100; i++){
    sparki_RGB(0,i,0);
    delay(time);
  }
  // make green dimmer
  for(int i=100; i>0; i--){
    sparki_RGB(0,i,0);
    delay(time);
  }

  // make blue brighter
  for(int i=0; i<100; i++){
    sparki_RGB(0,0,i);
    delay(time);
  }
  // make blue dimmer
  for(int i=100; i>0; i--){
    sparki_RGB(0,0,i);
    delay(time);
  }
}

int main(int argc, char *argv[])
{
    init();
    sparki_begin();
    setup();
    while (1) { loop(); }
    return 0;
}
