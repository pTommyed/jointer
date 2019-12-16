// timer1 registrs -- 30 Hz 

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
int preload_timer = 62411;// preload timer 65536-16MHz/256/5Hz --- 256 is prescaler; 65563 - 16 bit counter(Timer1); 20Hz - requiered frequency --- 62411 ; 10 hz = 59286; 5hz =53036 ; 2hz=34286

void setup(){
  serial_initial();
  VL53L1X_initial();
  CAN_initial();

  timer1_init();  // setting of timer1
  
  Serial.println("Inicializace -- OK");

}
void loop(){
  VL53L1X_measuring();
  if (timmer_flag == true) {
    sensor.stopContinuous();
    timmer_flag = false;
    //Serial.print("distance[mm] : ");
    //Serial.println(distance);
    distance_message();
    CAN.sendMsgBuf(0x00 + can_adress_distance, 0, 4, buf_distance);
    sensor.startContinuous(30);
  }
}

