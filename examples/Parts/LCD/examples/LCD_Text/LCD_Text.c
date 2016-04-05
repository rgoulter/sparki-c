/*******************************************
 Drawing some text

 http://arcbotics.com/products/sparki/parts/lcd/
********************************************/
#include <Sparki.h>  // include the sparki library

void setup()
{
}

void loop()
{
    sparki_clearLCD(); // wipe the LCD clear
    sparki_drawChar(10, 1, 'a');
    sparki_drawChar(20, 2, 'b');
    sparki_drawChar(30, 3, 'c');

    sparki_drawString(40, 4, "123");
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

