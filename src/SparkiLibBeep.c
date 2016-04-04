#include "Sparki.h"



void begin_beep() {
    // Setup Buzzer
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);
}

void sparki_beep(){
    //R tone is C++, so can't use in pure C.
    // tone(BUZZER, BUZZER_FREQ, 200);
}

void sparki_beep_freq(int freq){
    //R tone is C++, so can't use in pure C.
    // tone(BUZZER, freq, 200);
}

void sparki_beep_freq_time(int freq, int time){
    //R tone is C++, so can't use in pure C.
    // tone(BUZZER, freq, time);
}

void sparki_noBeep(){
    //R tone is C++, so can't use in pure C.
    // noTone(BUZZER);
}
