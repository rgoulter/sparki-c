void begin_beep() {
    // Setup Buzzer
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);
}

void SparkiClass::beep(){
    tone(BUZZER, BUZZER_FREQ, 200);
}

void SparkiClass::beep(int freq){
    tone(BUZZER, freq, 200);
}

void SparkiClass::beep(int freq, int time){
    tone(BUZZER, freq, time);
}

void SparkiClass::noBeep(){
    noTone(BUZZER);
}
