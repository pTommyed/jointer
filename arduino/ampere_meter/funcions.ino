/*---------------------- Mereni proudu 20A sondou ------------------------------------*/

float proud20A (byte pin) {

  int NamerenyProud;  // vstup z proudove sondy
  float CurrentSouc;  // pomocna promena pro vypocet prumeru
  float Current;  // vysledny proud

  CurrentSouc = 0;
  
  for (int i = 0; i < 10; i++) {
    NamerenyProud = analogRead(pin);
    CurrentSouc = CurrentSouc + NamerenyProud;
  }

  CurrentSouc = CurrentSouc / 10;
 
  Current = CurrentSouc * (5.0 / 1023.0);
  Current = Current - (2.5); // Pro 0 A sonda vraci 2.5 V
  Current = Current * 10.0; // Protoze je 100 mV/A

  return abs(Current);
}

/*-------------------------- SyrenaDown ------------------------------------------------------------------------------*/

void BeepDown () {
  
  Timer1.stop();
  Timmer1End = true;
}

/*-------------------------- SyrenaUP ------------------------------------------------------------------------------*/

void BeepUP () {
  
  digitalWrite(led, HIGH);
  Timer1.initialize(300000);
}

/*------------------------ timer1 - overflow -----------------------------------------------------*/

void timer_overflow(){
  timmer_flag = true;
}

/*----------------------- CAN Message create ------------------------------------------------------*/

void create_message(){
  int current = celk_proud * 1000; // proud v mA
  float pom;

  for (int i=0;i<3;i++){
    buf[i]= current / pow(256,(2-i));
    current = current - (current * pow(256,(2-i)));
    //Serial.print(pow(256,(2-i)));
  }
}

