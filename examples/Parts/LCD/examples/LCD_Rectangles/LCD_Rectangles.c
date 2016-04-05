/*******************************************
 Drawing two rectangles

 http://arcbotics.com/products/sparki/parts/lcd/
********************************************/
#include <Sparki.h>  // include the sparki library

void setup()
{
}

void loop()
{
    sparki_clearLCD(); // wipe the LCD clear
    sparki_drawRect(5,5,30,10); //small empty rectangle in upper-left
    sparki_drawRectFilled(20,20,100,50); // big rectangle in lower-right
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
