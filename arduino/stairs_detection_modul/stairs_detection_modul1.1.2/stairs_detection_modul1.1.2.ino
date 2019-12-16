// timer 2 --- 30Hz senzor
#include <VL53L1X.h>
#include <SPI.h> // knihovna pro SPI sbernici
#include <mcp_can.h>
#include <Wire.h>

VL53L1X sensor;
unsigned int distance =0; // distance in [mm]

MCP_CAN CAN(10);
int           CANmessageID;
unsigned char len = 0;

byte buf_distance[4]={0,0,0,0};

int can_adress_distance = 113; //71 hexa  // Nutné ověřit !!!!

bool timmer_flag = false;
int preload_timer = 0;//Timer 2
int max_count_overflow = 23; // 20 hz
int count_overflow = 0;

void setup(){
  serial_initial();
  VL53L1X_initial();
  CAN_initial();

  timer2_init();  // setting of timer1
  
  Serial.println("Inicializace -- OK");

}
void loop(){
  VL53L1X_measuring();
  //Serial.println(timmer_flag);
  if (timmer_flag == true) {
    sensor.stopContinuous();
    timmer_flag = false;
    //Serial.print("distance[mm] : ");
    //Serial.println(distance);
    distance_message();
    //TCNT2 = preload_timer;
    CAN.sendMsgBuf(0x00 + can_adress_distance, 0, 4, buf_distance);
    sensor.startContinuous(30);
  }
}

