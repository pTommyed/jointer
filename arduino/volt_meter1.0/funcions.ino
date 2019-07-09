/*---------------------- Mereni napeti ------------------------------------*/

float voltage_measure (byte pin) {

  int voltage_pom;  // vstup
  float volt_count;  // pomocna promena pro vypocet prumeru
  float voltage_out;  // vysledny nap

  voltage_pom = 0;
  volt_count=0;
  
  for (int i = 0; i < 10; i++) {
    voltage_pom = analogRead(pin);
    volt_count = volt_count + voltage_pom;
  }

  volt_count = volt_count / 10;
  //Serial.println(volt_count,5);
 
  voltage_out = volt_count * (5.0 / 1024.0);
  voltage_out = (voltage_out / (R2/(R1+R2)));
  voltage_out = voltage_out-(voltage_out *0.07);
  

  Serial.println(voltage_out,3);
  return (voltage_out);
}

/*------------------------ timer1 - overflow -----------------------------------------------------*/

void timer_overflow(){
  timmer_flag = true;
}

/*----------------------- CAN Message voltage ------------------------------------------------------*/

void voltage_message(){
  int voltage_pom = celk_voltage * 1000; // napětí v mV
  float pom;
  int ground = 256;

  for (int i=0;i<4;i++){
    buf_voltage[i]= voltage_pom / power(ground,(3-i));
    voltage_pom = voltage_pom - (buf_voltage[i] * power(ground,(3-i)));
    //Serial.print(power(ground,(2-i)));
  }
}

/*--------------------- power ----------------------------------------------------------------------*/

float power (int ground ,int exponent) {
  float compute = 0;
  if (exponent > 1) {
    compute = ground;
    for (int i=0; i<(exponent-1);i++) {
      compute = compute * ground;
    }
   } else if (exponent == 0) {
        compute = 1;
     } else {
          compute = ground;
       }
  return(compute);
}
