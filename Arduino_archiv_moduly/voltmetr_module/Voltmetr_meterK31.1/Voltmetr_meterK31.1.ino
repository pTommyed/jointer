//####################################################
//#    KLOUBAK: Voltage_angle_measuring_module (Voltmetr_meterK31.1)
//#    Copyright (c) 2019 ROBOTIKA
//#    programmed by: Tomas Picha
//####################################################

// DESCRTIPTION: -  version for K3 robot 
//               -  no tested 
//               -  20 Hz - sampling frequency
//               -  module measure voltage - 12, 36V and angle of rotation (analog value)
//               -  refactoring of version - Voltmetr_meterK21.0 + new update for K3
// HW: Aruino Nano, DOPSAT NAZEV SENZORU PRO MERENI UHLU, MCP2515+TJA1050 CAN module

/*----------------------- DEPENDENCES ----------------------------------*/
#include <TimerOne.h>
#include <mcp_can.h>

/*----------------------- DEFINITION -----------------------------------*/
MCP_CAN CAN(10);
const int potenciometr_pin = A0;
const int voltege12_pin = A2;
const int voltege36_pin = A1;

float R1_12 = 470000.0;  //value of resistance of resistor in voltage divider
float R2_12 = 220000.0; //value of resistance of resistor in voltage divider
float R1_36 = 674000.0; //value of resistance of resistor in voltage divider  
float R2_36 = 67000.0; //value of resistance of resistor in voltage divider

int CANmessageID;
unsigned char len = 0;
byte buf_voltage12[4]={0,0,0,0};
byte buf_voltage36[4]={0,0,0,0};
byte buf_resistance[4] = {0,0,0,0};

int can_adress_resistance = 128; //80 hexa
int can_adress_voltage12 = 129;  //81 hexa
int can_adress_voltage36 = 130;  //82 hexa

unsigned int resist_potnciometr = 0;
float voltage12 = 0.0; 
float voltage36 = 0.0; 

bool timmer_flag = false;

/*----------------------- SETUP ----------------------------------------*/
void setup(){
  serial_initial();
  CAN_initial();
  timer1_init();
  Serial.println("initialization successfully done!");

}

/*----------------------- LOOP ------------------------------------------*/
void loop(){
  if (timmer_flag == true) {
     voltage12 =  Voltage (voltege12_pin,R1_12, R2_12);
     voltage36 =  Voltage (voltege36_pin,R1_36, R2_36);
     timmer_flag = false;
     voltage_message12();
     CAN.sendMsgBuf(0x00 + can_adress_voltage12, 0, 4, buf_voltage12);
     voltage_message36();
     CAN.sendMsgBuf(0x00 + can_adress_voltage36, 0, 4, buf_voltage36);
     resist_potnciometr = potenciomter(potenciometr_pin);
     resistance_message();
     CAN.sendMsgBuf(0x00 + can_adress_resistance, 0, 4, buf_resistance);
     //Serial.print("Current = ");
     //Serial.println(celk_proud,3);
  }
}
