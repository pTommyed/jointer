
byte reset_pin = 2;
<<<<<<< HEAD
int delay_time_up = 2000;
int delay_time_down = 5000;
=======
int delay_time = 2000;
>>>>>>> 2c8e97de6965b20be3ca725492db791018107e7a

void setup() {
  pinMode(reset_pin,OUTPUT);
  digitalWrite(reset_pin,HIGH);
<<<<<<< HEAD
  delay(delay_time_up);
  digitalWrite(reset_pin,LOW);
  delay(delay_time_down);
  digitalWrite(reset_pin,HIGH);
=======
  delay(delay_time);
 //digitalWrite(reset_pin,LOW);
>>>>>>> 2c8e97de6965b20be3ca725492db791018107e7a
}

void loop() {
  
}
