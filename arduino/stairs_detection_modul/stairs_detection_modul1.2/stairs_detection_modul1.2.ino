// Timer 1 with lib.
#include <VL53L1X.h>
#include <SPI.h> // knihovna pro SPI sbernici
#include <mcp_can.h>
#include <Wire.h>
#include <TimerOne.h>

VL53L1X sensor;
unsigned int distance =0; // distance in [mm]

MCP_CAN CAN(10);
int           CANmessageID;
unsigned char len = 0;

byte buf_distance[4]={0,0,0,0};

int can_adress_distance = 112; //70 hexa  // Nutné ověřit !!!!

bool timmer_flag = false;

void setup(){
  serial_initial();
  VL53L1X_initial();
  CAN_initial();

  Timer1.initialize(50000);
  Timer1.attachInterrupt(timer_overflow);
  
  Serial.println("Inicializace -- OK");

}
void loop(){
  VL53L1X_measuring();
  Serial.println(timmer_flag);
  if (timmer_flag == true) {
    //Serial.print("distance[mm] : ");
    timmer_flag = false;
    //Serial.println(distance);
    distance_message();
    CAN.sendMsgBuf(0x00 + can_adress_distance, 0, 4, buf_distance);
  }
}

