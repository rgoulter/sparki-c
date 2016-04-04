void begin_beep() {
    // Setup Buzzer
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);
}

void sparki_beep(){
    tone(BUZZER, BUZZER_FREQ, 200);
}

void sparki_beep_freq(int freq){
    tone(BUZZER, freq, 200);
}

void sparki_beep_freq_time(int freq, int time){
    tone(BUZZER, freq, time);
}

void sparki_noBeep(){
    noTone(BUZZER);
}
