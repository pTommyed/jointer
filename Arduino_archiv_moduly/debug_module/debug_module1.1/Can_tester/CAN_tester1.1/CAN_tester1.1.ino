// timer1
// DESCRTIPTION: -
// HW: Aruino Nano, buzzer, 8xled indication, MCP2515+TJA1050 CAN module

/*----------------------- DEPENDENCES ----------------------------------*/
#include <SPI.h>
#include <mcp_can.h>
#include <TimerOne.h>
#include <avr/wdt.h>        // library for default watchdog functions
#include <avr/interrupt.h>  // library for interrupts handling

/*----------------------- DEFINITION -----------------------------------*/
const byte led_pinout_count = 8;  // count of led pins 
const byte buzzer_pin = 17;
const byte cs_pin = 10;
byte led_pinout[led_pinout_count]= {7,6,4,3,5,9,2,8};

//  adress_tabel {CAN_adress,position in led_pinout field,reading_CAN_mesage(1-read),buzzer_ON/OFF (1-ON,0-oFF),recived_message(1/0)}

const int record_number = 12; // number of watched CAN adress
int adress_tabel [record_number][5]= {{145,0,0,0,0}, // vesc 1 2305
                                     {146,0,0,0,0}, // vesc 2
                                     {147,1,0,0,0}, // vesc 3 
                                     {148,1,0,0,0}, // vesc 4
                                     {149,2,0,0,0}, // vesc 5
                                     {150,2,0,0,0}, // vesc 6
                                     {129,4,0,0,0}, // voltage 12V
                                     {130,4,1,1,0}, // voltage 46 V
                                     {131,7,0,0,0}, // tacho_modul
                                     {113,5,0,0,0}, // down_drop_front_modul
                                     {114,6,0,0,0}, // down_drop_back_modul
                                     {1,3,0,0,0}}; // junction modul Stop_button
                                     
MCP_CAN CAN(cs_pin);
unsigned int CANmessageID = 0;
unsigned char len = 0;
byte buf[8];
unsigned char message_to_apu_len = 8;
byte message_to_apu[8]={0,0,0,0,0,0,0,0};
int can_adress = 116; //74 hexa 

bool timmer_flag = false;
byte cycle_count = 0;

/*----------------------- SETUP ----------------------------------------*/
void setup() {
  serial_initial();
  CAN_initial();
  CAN_MASK_initial();
  //CAN_filter_extended_initial();
  CAN_filter_standard_initial(); 
  pinout_initial();
  Beep();
  knight_rider_led_test ();
  //timer1_initial();
  WDT_1s();
  Serial.println(" - initialization successfully done!");
}

/*----------------------- MAIN LOOP ----------------------------------------*/

void loop() {

  if (timmer_flag == true) {
    //Serial.println(timmer_flag);
    timer_service();
    /*led_reset();
    detection_missing_module();
    sending_message_to_apu();
    missing_module_status_reset();*/
    if (cycle_count == 1) {
      //CAN_filter_standard_vesc_initial();
    } else if (cycle_count == 2) {
              //CAN_filter_extended_initial();
              //CAN_filter_standard_initial();
              led_reset();
              detection_missing_module();
              sending_message_to_apu();
              missing_module_status_reset();
             }
  }
  if(CAN.readMsgBuf(&len, buf)== CAN_OK) {  
    CANmessageID = CAN.getCanId();
    CAN_message_received();
  }

}


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

/*-----------------------CAN-MASK-initialization--------------------------------*/
void CAN_MASK_initial(){
  /*CAN.init_Mask(0, 1, 0x3ffff); //filtruje extended zprávy a nekomtroluje první dva bajty adresy CAN.init_Mask(0, 1, 0x3ff00);
  CAN.init_Mask(0, 1, 0x3ffff); // CAN.init_Mask(0, 1, 0x3ffff) filtruje extended a kontorluje všechny bjaty adresy
  */
   //CAN.init_Mask(0, 0, 0x700); //filtruje extended zprávy a nekomtroluje první dva bajty adresy CAN.init_Mask(0, 1, 0x3ff00);
  CAN.init_Mask(0, 0, 0x7ff); //filtruje extended zprávy a nekomtroluje první dva bajty adresy CAN.init_Mask(0, 1, 0x3ff00);
  CAN.init_Mask(1, 1, 0x1fffffff); // CAN.init_Mask(0, 1, 0x3ffff) filtruje extended a kontorluje vše
}

/*-----------------------CAN-filter-standard_vesc-initialization--------------------------------*/
void CAN_filter_standard_vesc_initial(){ 
  
  CAN.init_Filt(0, 1, 0x01450000);  //901
  CAN.init_Filt(1, 1, 0x01460000);
  CAN.init_Filt(2, 1, 0x01470000); //903
  CAN.init_Filt(3, 1, 0x01480000);
  CAN.init_Filt(4, 1, 0x01490000);
  CAN.init_Filt(5, 1, 0x01500000);
}

/*-----------------------CAN-filter-extended-initialization--------------------------------*/
void CAN_filter_extended_initial(){ 
  CAN.init_Mask(0, 1, 0x3ffff); //filtruje extended zprávy a nekomtroluje první dva bajty adresy CAN.init_Mask(0, 1, 0x3ff00);
  CAN.init_Mask(1, 1, 0x3ffff); // CAN.init_Mask(0, 1, 0x3ffff) filtruje extended a kontorluje všechny bjaty adresy
  
  CAN.init_Filt(0, 1, 0x0901);  //901
  CAN.init_Filt(1, 1, 0x0902);
  CAN.init_Filt(2, 1, 0x0903); //903
  CAN.init_Filt(3, 1, 0x0904);
  CAN.init_Filt(4, 1, 0x0905);
  CAN.init_Filt(5, 1, 0x0906);
}

/*-----------------------CAN-filter-standard-initialization--------------------------------*/
void CAN_filter_standard_initial(){
  /*CAN.init_Mask(0, 0, 0x07ff); //filtruje extended zprávy a nekomtroluje první dva bajty adresy CAN.init_Mask(0, 1, 0x3ff00);
  CAN.init_Mask(1, 0, 0x07ff); // CAN.init_Mask(0, 1, 0x3ffff) filtruje extended a kontorluje všechny bjaty adresy
  */
     
  CAN.init_Filt(0, 0, 0x01);  //0x1
  CAN.init_Filt(1, 0, 0x71);
  CAN.init_Filt(2, 0, 0x81); //903
  CAN.init_Filt(3, 0, 0x81);
  CAN.init_Filt(4, 0, 0x01);
  CAN.init_Filt(5, 0, 0x01);

  /*CAN.init_Filt(0, 0, 0x0020);  //0x1
  CAN.init_Filt(1, 0, 0x071);
  CAN.init_Filt(2, 0, 0x072); //903
  CAN.init_Filt(3, 0, 0x081);
  CAN.init_Filt(4, 0, 0x082);
  CAN.init_Filt(5, 0, 0x093);*/
}

/*-------------------------- SyrenaUP ------------------------------------------------------------------------------*/

void Beep () {
  
  digitalWrite(buzzer_pin, HIGH);
  delay(500);
  digitalWrite(buzzer_pin, LOW);
}

/*-------------------------- timer1_initial ------------------------------------------------------------------------------*/

/*void timer1_initial() {
  
  Timer1.initialize(1000000); //1Hz//10 Hz freq = 100000
  Timer1.attachInterrupt(timer_overflow);
}
*/
/*-------------------------- Timer 1 overflow ----------------*/

ISR( WDT_vect ) {
  MCUSR &= ~(1 << WDRF);                           // reset watch dog
  //SPCR = 00010000;; //SPCR = SPCR & 0x7F;
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
  int can_adress_pom=30;
  Serial.println( CANmessageID);
  if ((CANmessageID > 144) && (CANmessageID < 151)){
    can_adress_pom = CANmessageID - 145;
    adress_tabel[can_adress_pom][4]=1;
  }else {
      if (CANmessageID < 132){
        if (CANmessageID > 128){
          can_adress_pom = (CANmessageID - 129)+6;
          adress_tabel[can_adress_pom][4]=1;
        } else if ((CANmessageID > 112) && (CANmessageID < 115)) {
            can_adress_pom = (CANmessageID - 113)+9;
            adress_tabel[can_adress_pom][4]=1;
          } else if (CANmessageID == 1){
                can_adress_pom = (CANmessageID - 1)+11;
                adress_tabel[can_adress_pom][4]=1;
            }
      }    
   }
  Serial.println( can_adress_pom);
}

/*------------------------- detection_missing_module -----------------------------------------------------------------------------------*/

void detection_missing_module () {
  for (int i = 0; i < record_number; i++){
    if ( adress_tabel[i][4]== 0) {
      digitalWrite(led_pinout[adress_tabel[i][1]], HIGH);
       message_to_apu[led_pinout[adress_tabel[i][1]]] = led_pinout[adress_tabel[i][1]]; 
    } else {
          message_to_apu[led_pinout[adress_tabel[i][1]]] = 0; 
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

/*------------------------ WDT 1 S -----------------------------------------*/

void WDT_1s(){
  MCUSR &= ~(1 << WDRF);                           // reset watch dog
  WDTCSR |= (1 << WDCE) | (1 << WDE);              // povolení konfigurace watch dog
  WDTCSR = (1 << WDP1) | (1 << WDP2) ;             // nastavení watch dog na 1 s
  WDTCSR |= (1 << WDIE);                           // enable interrupt mode

}

/*------------------------ WDT 2 S -----------------------------------------*/

void WDT_2s(){
  MCUSR &= ~(1 << WDRF);                           // reset watch dog
  WDTCSR |= (1 << WDCE) | (1 << WDE);              // povolení konfigurace watch dog
  WDTCSR = (1 << WDP0) | (1 << WDP1) | (1 << WDP2) ;             // nastavení watch dog na 2 s
  WDTCSR |= (1 << WDIE);                           // enable interrupt mode

}

/*------------------------ WDT 0.5 S -----------------------------------------*/

void WDT_05s(){
  MCUSR &= ~(1 << WDRF);                           // reset watch dog
  WDTCSR |= (1 << WDCE) | (1 << WDE);              // povolení konfigurace watch dog
  WDTCSR = (1 << WDP0) | (1 << WDP2);             // nastavení watch dog na 0.5 s
  WDTCSR |= (1 << WDIE);                           // enable interrupt mode

}