/*******************************************
 Drawing some text

 http://arcbotics.com/products/sparki/parts/lcd/

 C port
********************************************/
#include <Sparki.h>  // include the sparki library

int main(int argc, char **argv) {
    sparki_begin();

    int i = 0;
    while (1) {
        sparki_clearLCD(); // wipe the LCD clear
        /* sparki_drawChar(10, 1, 'a'); */
        /* sparki_drawChar(20, 2, 'b'); */
        /* sparki_drawChar(30, 3, 'c'); */
        /*  */
        /* sparki_drawString(40, 4, "123"); */
        for (int j = 0; j <= i; j++) {
            sparki_textWrite("hello world\n", 13);
        }

        i += 1;
        if (i > 3) {
            i = 0;
        }

        sparki_updateLCD(); // put the drawings on the screen
        delay(1);
    }
}


