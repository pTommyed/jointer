
byte reset_pin = 2;
int delay_time_up = 2000;
int delay_time_down = 5000;

void setup() {
  pinMode(reset_pin,OUTPUT);
  digitalWrite(reset_pin,HIGH);
  delay(delay_time_up);
  digitalWrite(reset_pin,LOW);
  delay(delay_time_down);
  digitalWrite(reset_pin,HIGH);
}

void loop() {
  
}
