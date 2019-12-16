/*-----------------------Serial-initialization--------------------------------*/
void serial_initial(){
  Serial.begin(115200); 
  while (!Serial) {
    ; // ceka dokud neni pripojena seriova linka
    }
}

/*-----------------------Pinout-initialization--------------------------------*/
void pinout_initial(){
  for (int i=0; i<8;i++) {
    pinout[i]= i+2;
    pinMode(pinout[i], OUTPUT);
    digitalWrite(pinout[i],LOW);
  }
  for (int i=8; i<14;i++) {
    pinout[i]= i+6;
    pinMode(pinout[i], OUTPUT);
    digitalWrite(pinout[i],LOW);
  }
}

/*-----------------------Pinout-testing--------------------------------*/
void pinout_testing(){
  for (int i=0; i<14;i++) {
    digitalWrite(pinout[i],HIGH);
    Serial.print("Pins number : ");
    Serial.println(pinout[i]);
    delay(5000);
    digitalWrite(pinout[i],LOW);
  }
}
