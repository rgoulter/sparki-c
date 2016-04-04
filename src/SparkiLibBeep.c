void begin_beep() {
    // Setup Buzzer
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);
}

void sparki_beep(){
    tone(BUZZER, BUZZER_FREQ, 200);
}

void sparki_beep(int freq){
    tone(BUZZER, freq, 200);
}

void sparki_beep(int freq, int time){
    tone(BUZZER, freq, time);
}

void sparki_noBeep(){
    noTone(BUZZER);
}
