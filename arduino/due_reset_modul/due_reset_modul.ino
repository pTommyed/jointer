
byte reset_pin = 2;
int delay_time = 2000;

void setup() {
  pinMode(reset_pin,OUTPUT);
  digitalWrite(reset_pin,HIGH);
  delay(delay_time);
 //digitalWrite(reset_pin,LOW);
}

void loop() {
  
}
