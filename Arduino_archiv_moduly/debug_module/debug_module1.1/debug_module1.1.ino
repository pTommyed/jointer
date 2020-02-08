//####################################################
//#    KLOUBAK: Debuger Module
//#    Copyright (c) 2019 ROBOTIKA
//#    programmed by: Tomas Picha, Jan Kaderabek
//####################################################

// timer1
// DESCRTIPTION: -
// HW: Aruino Nano, buzzer, 8xled indication, MCP2515+TJA1050 CAN module

/*----------------------- DEPENDENCES ----------------------------------*/
#include <SPI.h>
#include <mcp_can.h>
#include <TimerOne.h>

/*----------------------- DEFINITION -----------------------------------*/
const byte led_pinout_count = 8;  // count of led pins 
const byte buzzer_pin = 17;
const byte cs_pin = 10;
byte led_pinout[led_pinout_count]= {7,6,4,3,5,9,2,8};

//  adress_tabel {CAN_adress,position in led_pinout field,reading_CAN_mesage(1-read),buzzer_ON/OFF (1-ON,0-oFF),recived_message(1/0)}

const int record_number = 14; // number of watched CAN adress
int adress_tabel [record_number][5]= {{2305,7,0,0,0}, // vesc 1
                                     {2306,7,0,0,0}, // vesc 2
                                     {2307,6,0,0,0}, // vesc 3 
                                     {2308,6,0,0,0}, // vesc 4
                                     {2309,4,0,0,0}, // vesc 5
                                     {2310,4,0,0,0}, // vesc 6
                                     {128,5,0,0,0}, // angle
                                     {129,5,0,0,0}, // voltage 12V
                                     {130,5,1,1,0}, // voltage 46 V
                                     {131,8,0,0,0}, // tacho_modul
                                     {113,9,0,0,0}, // down_drop_front_modul
                                     {114,2,0,0,0}, // down_drop_back_modul
                                     {1,3,0,0,0}, // junction modul Stop_button
                                     {2,3,0,0,0}}; // junction modul counter
                                     
MCP_CAN CAN(cs_pin);
unsigned int CANmessageID;
unsigned char len = 0;
unsigned char message_to_apu_len = 8;
byte message_to_apu[8]={0,0,0,0,0,0,0,0};
int can_adress = 116; //73 hexa 

bool timmer_flag = false;
byte cycle_count = 0;

/*----------------------- SETUP ----------------------------------------*/
void setup() {
  serial_initial();
  CAN_initial();
  CAN_filter_initial();
  pinout_initial();
  Beep();
  knight_rider_led_test ();
  timer1_initial();
  Serial.println(" - initialization successfully done!");
}

/*----------------------- MAIN LOOP ----------------------------------------*/

void loop() {

  if (timmer_flag == true) {
    timer_service();
    detection_missing_module();
    sending_message_to_apu(); 
    if (cycle_count == 2) {
      missing_module_status_reset();
      led_reset();
    }
  }
  
  if(CAN_MSGAVAIL == CAN.checkReceive()) {  
    CANmessageID = CAN.getCanId();
    CAN_message_received();
  }

}
