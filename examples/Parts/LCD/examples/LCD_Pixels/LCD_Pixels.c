/*******************************************
 Drawing some pixels

 In this case, a tiny smiley face in the
 upper left hand corner

 http://arcbotics.com/products/sparki/parts/lcd/
********************************************/
#include <Sparki.h>  // include the sparki library

void setup()
{
}

void loop()
{
    sparki_clearLCD(); // wipe the LCD clear
    //draw a smiley face in the upper left hand corner

    // left eye
    sparki_drawPixel(4, 4);
    sparki_drawPixel(4, 5);
    sparki_drawPixel(5, 4);
    sparki_drawPixel(5, 5);

    // right eye
    sparki_drawPixel(10, 4);
    sparki_drawPixel(10, 5);
    sparki_drawPixel(11, 4);
    sparki_drawPixel(11, 5);

    //smile
    sparki_drawPixel(4, 8);
    sparki_drawPixel(5, 9);
    sparki_drawPixel(6, 10);
    sparki_drawPixel(7, 11);
    sparki_drawPixel(8, 11);
    sparki_drawPixel(9, 10);
    sparki_drawPixel(10, 9);
    sparki_drawPixel(11, 8);

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

