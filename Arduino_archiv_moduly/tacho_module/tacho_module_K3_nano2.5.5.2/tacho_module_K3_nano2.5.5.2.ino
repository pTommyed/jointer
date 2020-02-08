//####################################################
//#    KLOUBAK: Tacho Module K3 2.5.5.2
//#    Copyright (c) 2019 ROBOTIKA
//#    programmed by: Tomas Picha, Martin Dlouhy
//####################################################


// DESCRTIPTION: -  version for K3 robot 
//               -  no tested
//               -  20 Hz - sampling frequency
//               -  timer1 registrs 
//               -  new CAN_mask and filter settings 
// HW: Aruino Nano, MCP2515+TJA1050 CAN module

// version -- if statement for vesc canID and new tacho_buf indexing and timer change to 20 Hz

/*----------------------- DEPENDENCES ----------------------------------*/
#include <mcp_can.h>
#include <SPI.h> // lib for SPI bus

/*----------------------- DEFINITION -----------------------------------*/
const int can_vesc_count = 6; // count of vescs on the robot
int CANvescID[can_vesc_count] = {1,2,3,4,5,6};  // motors ID

MCP_CAN CAN(10);

int can_adress_transmit = 131; // can adress 0X83
int  CANmessageID; // can adress of incoming message
byte buf[8];
unsigned char len = 0;
byte tacho_buf[6] = {0,0,0,0,0,0};
byte request_buf[3] = {0,0,4}; 

byte cycle_count = 0;

bool Timer1Over = false; //indication of timer overflow
int preload_timer = 62411;// preload timer 65536-16MHz/256/5Hz --- 256 is prescaler; 65563 - 16 bit counter(Timer1); 20Hz - requiered frequency --- 62411 ; 10 hz = 59286; 5hz =53036 ; 2hz=34286

/*----------------------- SETUP ----------------------------------------*/
void setup() {
 
  serial_initial();
  CAN_initial();
 
  timer1_init();  // setting of timer1

  Serial.println("initialization done!");
}

/*----------------------- LOOP ------------------------------------------*/
void loop() {
  if (Timer1Over == true){
    Timer1Over = false;
    for (int i=0;i<can_vesc_count;i++){
      tacho_service(i);
      sending_tacho_apu(i);
    }
    cycle_count_service();    
  }
}
