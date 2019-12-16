/*-----------------------Serial-initialization--------------------------------*/
void serial_initial(){
  Serial.begin(115200); 
  while (!Serial) {
    ; // ceka dokud neni pripojena seriova linka
    }
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

/*-----------------------CAN-initialization--------------------------------*/
void CAN_filter_initial(){
  CAN.init_Mask(0, 1, 0x00000910); 
  CAN.init_Mask(1, 1, 0x00000910); 
  
  CAN.init_Filt(0, 1, 0x00000901);
  CAN.init_Filt(1, 1, 0x00000902);
  CAN.init_Filt(2, 1, 0x00000903);
  CAN.init_Filt(3, 1, 0x00000904);
  CAN.init_Filt(4, 1, 0x00000905);
  CAN.init_Filt(5, 1, 0x00000906);
}

/*-------------------------- SyrenaUP ------------------------------------------------------------------------------*/

void Beep () {
  
  digitalWrite(buzzer_pin, HIGH);
  delay(500);
  digitalWrite(buzzer_pin, LOW);
}

/*-------------------------- timer1_initial ------------------------------------------------------------------------------*/

void timer1_initial() {
  
  Timer1.initialize(1000000); //1Hz//10 Hz freq = 100000
  Timer1.attachInterrupt(timer_overflow);
}

/*-------------------------- Timer 1 overflow ----------------*/

void timer_overflow() {
  timmer_flag = true;
}

/*--------------------------timer_service--------------------*/

void timer_service(){
  timmer_flag = false;
  cycle_count = cycle_count +1;
}

/*-----------------------Pinout-initialization--------------------------------*/
void pinout_initial(){
  for (int i=0; i<led_pinout_count ;i++) {
    pinMode(led_pinout[i], OUTPUT);
    digitalWrite(led_pinout[i],LOW);
  }
  pinMode(buzzer_pin, OUTPUT);
  digitalWrite(buzzer_pin,LOW);
}

/*-----------------------Knight Rider led test--------------------------------*/
void knight_rider_led_test (){

  for (int j = 0; j<5; j++) {
    for (int i=0; i<led_pinout_count ;i++) {
      digitalWrite(led_pinout[i],HIGH);
      delay(50);
      if (i < (led_pinout_count-1)){
       digitalWrite(led_pinout[i],LOW); 
      }
    }
    for (int i=led_pinout_count; i>=0;i=i-1) {
        digitalWrite(led_pinout[i],HIGH);
        delay(50);
        digitalWrite(led_pinout[i],LOW);
      }
  }
}

/*---------------------------  CAN_message_received ----------------------------------------------------------------------------------*/

void CAN_message_received(){
  int can_adress;
  if (CANmessageID > 2304 && CANmessageID < 2311){
    can_adress = CANmessageID - 2305;
    adress_tabel[can_adress][4]=1;
  }else {
      if (CANmessageID < 131){
        if (CANmessageID > 127){
          can_adress = (CANmessageID - 128)+6;
          adress_tabel[can_adress][4]=1;
        } else if (CANmessageID > 112 && CANmessageID < 115 ) {
            can_adress = (CANmessageID - 113)+9;
            adress_tabel[can_adress][4]=1;
          } else if (CANmessageID < 3) {
                can_adress = (CANmessageID - 1)+12;
                adress_tabel[can_adress][4]=1;
            }
      }    
   }
}

/*------------------------- detection_missing_module -----------------------------------------------------------------------------------*/

void detection_missing_module () {

  for (int i = 0; i < record_number; i++){
    if ( adress_tabel[i][4]== 0) {
      digitalWrite(adress_tabel[i][1], HIGH);
      message_to_apu[i] = 1; 
    }
  }
}

/*----------------------------- missing_module_status_reset -----------------------------------------------------------------------------------------------*/

void missing_module_status_reset() {
  cycle_count = 0;
  for (int i = 0; i < record_number; i++){
    adress_tabel[i][4] = 0;
     }
}

/*----------------------------- led_reset -----------------------------------------------------------------------------------------------*/

void led_reset() {
  for (int i=0; i<led_pinout_count ;i++) {
      digitalWrite(led_pinout[i],LOW);  
   }
}

/*----------------------------- Sending_message_to_apu -----------------------------------------------------------------------------------------------*/

void sending_message_to_apu() {
  CAN.sendMsgBuf(0x00 + can_adress, 0, message_to_apu_len,  message_to_apu);
  for (int i=0; i<led_pinout_count ;i++) {
      message_to_apu[i]=0;  
   }
}

