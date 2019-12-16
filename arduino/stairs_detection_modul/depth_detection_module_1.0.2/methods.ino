/*-----------------------Serial-initialization--------------------------------*/
void serial_initial(){
  Serial.begin(115200); 
  while (!Serial) {
    ; // ceka dokud neni pripojena seriova linka
    }
}

/*-----------------------I2C-initialization-----------------------------------*/
void i2c_initial()
{
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C
}

/*--------------------- Sensor VL53L1X-initialization------------------------*/
void VL53L1X_initial()
{
  Serial.println("VL53L1X_setup...");
  pinMode(sensor_pin_A,OUTPUT);
  digitalWrite(sensor_pin_A,LOW);
  pinMode(sensor_pin_B,OUTPUT);
  digitalWrite(sensor_pin_B,LOW);
  delay(500);
    
  pinMode(sensor_pin_A, INPUT);
  delay(150);
  sensor_A.init(true);
  delay(100);
  sensor_A.setAddress((uint8_t)30);

  pinMode(sensor_pin_B, INPUT);
  delay(150);
  sensor_B.init(true);
  delay(100);
  sensor_B.setAddress((uint8_t)31);
  
  Serial.println(" - I2C addresses have been set");
  
  sensor_A.setTimeout(500);
  sensor_B.setTimeout(500);

  // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
  // You can change these settings to adjust the performance of the sensor, but
  // the minimum timing budget is 20 ms for short distance mode and 33 ms for
  // medium and long distance modes. See the VL53L1X datasheet for more
  // information on range and timing limits.
  sensor_A.setDistanceMode(VL53L1X::Long);
  sensor_B.setDistanceMode(VL53L1X::Long);
  sensor_A.setMeasurementTimingBudget(30000); //28185
  sensor_B.setMeasurementTimingBudget(30000); //28185

  // Start continuous readings at a rate of one measurement every 50 ms (the
  // inter-measurement period). This period should be at least as long as the
  // timing budget.
  sensor_A.startContinuous(30);
  sensor_B.startContinuous(30);
}

/*--------------------- I2C scanner -----------------------------------------*/
void i2c_scanner()
{
  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;
  for (byte i = 1; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      Serial.print (" - found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      delay (1); 
    }
  }
  Serial.print (" - found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");
}

/*-----------------------CAN-initialization--------------------------------*/
void CAN_initial(){
  Serial.println("Setup can..");
  while(CAN.begin(CAN_500KBPS, MCP_8MHz) != CAN_OK){
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nCAN init ok!!\r\n");
}

/*-------------------------- Probuzeni arduina od interniho preruseni ----------------*/

ISR(TIMER1_OVF_vect)  {
  TCNT1 = preload_timer;
  timmer_flag = true;
}

/*-------------------------- initialize timer1 ------------------------------------------*/

void timer1_init() {
  noInterrupts();
  
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = preload_timer;            // preload timer 65536-16MHz/256/20Hz
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
}

/*-----------------------VL53L1X-measuring--------------------------------*/
void VL53L1X_measuring(){
  sensor_A.read();
  distance_A = sensor_A.ranging_data.range_mm;
  sensor_B.read();
  distance_B = sensor_B.ranging_data.range_mm;
}

/*----------------------- CAN Message distance - create ------------------------------------------------------*/

void distance_message(){
  unsigned int distance_pom_A = distance_A; // distance[mm]
  unsigned int distance_pom_B = distance_B; // distance[mm]
  float pom;
  int ground = 256;


  /*Serial.print(distance_pom_A);
  Serial.print(" , ");
  Serial.print(distance_pom_B);
  Serial.println(" , ");*/
  
  for (int i=2;i<4;i++){
    buf_distance[i]= distance_pom_A / power(ground,(3-i));
    distance_pom_A = distance_pom_A - (buf_distance[i] * power(ground,(3-i)));
    //Serial.print(power(ground,(4-i)));
    //Serial.print(buf_distance[i]);
  }
  for (int i=0;i<2;i++){
    buf_distance[i]= distance_pom_B / power(ground,(1-i));
    distance_pom_B = distance_pom_B - (buf_distance[i] * power(ground,(1-i)));
    //Serial.print(power(ground,(4-i)));
    //Serial.print(buf_distance[i]);
  }
 // Serial.println("");
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

