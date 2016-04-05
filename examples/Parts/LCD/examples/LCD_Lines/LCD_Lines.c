/*******************************************
 Drawing a triangle using liness

 http://arcbotics.com/products/sparki/parts/lcd/
********************************************/
#include <Sparki.h>  // include the sparki library

void setup()
{
}

void loop()
{
    sparki_clearLCD(); // wipe the LCD clear
    sparki_drawLine(5,40,30,10);  //left side of trangle
    sparki_drawLine(30,10,55,40); // right side of triangle
    sparki_drawLine(5,40,55,40); // bottom of triangle
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
