// initialize the RGB timer variables
static volatile uint8_t RGB_vals[3];
static volatile uint16_t RGB_timer;

// TIMER4_COMPA_vect ISR
void int_RGB() {
}

void SparkiClass::RGB(uint8_t R, uint8_t G, uint8_t B)
{
    if(R > 100){
        R = 100;
    }
    if(G > 100){
        G = 100;
    }
    if(B > 100){
        B = 100;
    }
    RGB_vals[0] = int(R/2.0);
    RGB_vals[1] = int(G/2.0);
    RGB_vals[2] = int(B/2.0);
}
