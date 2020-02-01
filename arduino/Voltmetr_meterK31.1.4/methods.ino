/*----------------------I2C-initialization------------------------------------*/
void I2C_initial(){
  Wire.begin();
  Wire.setClock(400000);     // use 400 kHz I2C
}
/*----------------------I2C-scanner------------------------------------*/
void I2C_scanner()
{
  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;
  for (byte i = 1; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      delay (1); 
    }
  }
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");
}

/*----------------------Serial-initialization------------------------------------*/
void serial_initial(){
  Serial.begin(250000); 
  while (!Serial) {
      ; 
  }
}

/*----------------------CAN-initialization------------------------------------*/
void CAN_initial(){
  Serial.println("Setup can..");
  while(CAN.begin(CAN_500KBPS, MCP_8MHz) != CAN_OK){
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nCAN init ok!!\r\n");
}

/*-------------------------- WDT 16 ms ------------------------------------------*/

void WSDT_16ms(){
  MCUSR &= ~(1 << WDRF);                           // reset watch dog
  WDTCSR |= (1 << WDCE) | (1 << WDE);              // enable configuration of watch dog
  WDTCSR = (0 << WDP0) | (0 << WDP1) | (0 << WDP2) | (0 << WDP3); // setting watch dog to 16 ms
  WDTCSR |= (1 << WDIE);   // enable interrupt mode
}

/*----------------------36 Voltage meassure ------------------------------------*/

void Voltage_meassure_36 () {

  int meassure_voltage_36 = 0;  
  float voltage_count_36 = 0;  // pomocna promena pro vypocet prumeru
  
  for (int i = 0; i < 3; i++) {
    meassure_voltage_36 = analogRead(voltege36_pin);
    voltage_count_36 = voltage_count_36 + meassure_voltage_36;
  }

  voltage_count_36 = voltage_count_36 / 3;
 
  voltage36  = voltage_count_36 * (5.0 / 1024.0);
  voltage36  = (voltage36 / (R2_36/(R1_36+R2_36)));
  //voltage = (voltage-(voltage *0.07)); // korekce děliču - upravit 

  Serial.println(voltage36 ,3);
}

/*---------------------- 12 Voltage meassure ------------------------------------*/

void Voltage_meassure_12 () {

  int meassure_voltage_12 = 0;  
  float voltage_count_12 = 0;  // pomocna promena pro vypocet prumeru
  
  for (int i = 0; i < 3; i++) {
    meassure_voltage_12= analogRead(voltege12_pin);
    voltage_count_12 = voltage_count_12 + meassure_voltage_12;
  }

  voltage_count_12 = voltage_count_12 / 3;
 
  voltage12 = voltage_count_12 * (5.0 / 1024.0);
  voltage12 = (voltage12 / ( R2_12/(R1_12+ R2_12)));
  //voltage = (voltage-(voltage *0.07)); // korekce děliču - upravit 

  Serial.println(voltage12,3);
}

/*-------------------------- WDT overflow ----------------*/

ISR( WDT_vect ) {
  MCUSR &= ~(1 << WDRF); // reset watch dog
  WDT_overflow_count ++;
  if (WDT_overflow_count == 3) {
    timmer_flag = true;
    WDT_overflow_count = 0;
  } 
}

/*----------------------- CAN Message voltage12V ------------------------------------------------------*/

void voltage_message12(){
  int voltage_pom12 = voltage12  * 1000; // voltage in mV
  float pom;
  int ground = 256;
  
  for (int i=0;i<4;i++){
    buf_voltage12[i]= voltage_pom12 / power(ground,(3-i));
    voltage_pom12 = voltage_pom12 - (buf_voltage12[i] * power(ground,(3-i)));
  //Serial.print(power(ground,(4-i)));
    Serial.print(buf_voltage12[i]);
  }
  Serial.println("");
}

/*----------------------- CAN Message voltage36V ------------------------------------------------------*/

void voltage_message36(){
  int voltage_pom36 = voltage36  * 1000; // proud v mV
  float pom;
  int ground = 256;

  for (int i=0;i<4;i++){
    buf_voltage36[i]= voltage_pom36 / power(ground,(3-i));
    voltage_pom36 = voltage_pom36 - (buf_voltage36[i] * power(ground,(3-i)));
  //Serial.print(power(ground,(4-i)));
    Serial.print(buf_voltage36[i]);
  }
  Serial.println("");
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

/*-------------------- angle measure -----------------------------------------------------------------*/

void angle_measure (byte adrress) {
  
  Wire.beginTransmission(adrress);
  
  Wire.write(254);
  Wire.requestFrom(adrress,1); //adrress i2c
  angle_254 = Wire.read();
  
  Wire.write(255);
  Wire.requestFrom(adrress,1);
  angle_255 = Wire.read();
  
  Wire.endTransmission();
}

/*-------------------- angles measurement -----------------------------------------------------------------*/

void angles_measurement () {

  angle_measure(sensor1_i2c_adrress);
  buf_angles[0] = angle_254;
  buf_angles[1] = angle_255;

  angle_measure(sensor2_i2c_adrress);
  buf_angles[2] = angle_254;
  buf_angles[3] = angle_255;
  
}

/*-------------------- voltage measurement -----------------------------------------------------------------*/

void voltage_measurement (){
  Voltage_meassure_12();
  Voltage_meassure_36();
  voltage_message12();
  voltage_message36();
}

/*-------------------- CAN messages sending -----------------------------------------------------------------*/

void CAN_messages_sending () {
  CAN.sendMsgBuf(0x00 + can_adress_voltage12, 0, 4, buf_voltage12);
  CAN.sendMsgBuf(0x00 + can_adress_voltage36, 0, 4, buf_voltage36);
  CAN.sendMsgBuf(0x00 + can_adress_angles, 0, 4, buf_angles);

  variables_clearing ();
}

/*-------------------- variables clearing -----------------------------------------------------------------*/

void variables_clearing () {
  for (int i = 0; i < 4; i++) {
    buf_voltage12[i] = 0;
    buf_voltage36[i] = 0;
    buf_angles[i] = 0;
  }
  voltage12 = 0.0;
  voltage36 = 0.0;
}
