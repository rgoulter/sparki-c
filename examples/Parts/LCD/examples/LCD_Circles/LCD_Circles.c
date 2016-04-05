/*******************************************
 Drawing two circles

 http://arcbotics.com/products/sparki/parts/lcd/
********************************************/
#include <Sparki.h>  // include the sparki library

void setup()
{
}

void loop()
{
    sparki_clearLCD(); // wipe the LCD clear
    sparki_drawCircle(10,10,5); //small empty circle in upper-left
    sparki_drawCircleFilled(90,40,20); // big filled circle in lower-right
    sparki_updateLCD(); // put the drawings on the screen
    delay(1000);
}

int main(int argc, char *argv[])
{
    init();
    sparki_begin();
    setup();
    while(1) { loop(); }
    return 0;
}
