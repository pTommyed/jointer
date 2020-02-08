#include <TimerOne.h>
#include <mcp_can.h>

float celk_voltage; // nap v obvodu se zatezi

float R1 = 30000.0; //  
float R2 = 3300.0; //

MCP_CAN CAN(10);
int           CANmessageID;
unsigned char len = 0;
byte buf_voltage[4]={0,0,0,0};

int can_adress_current = 113;  //71 hexa

const int voltage_pin = A0;

bool timmer_flag = false;


void setup(){
  Serial.begin(250000); 
  while (!Serial) {
    ; // ceka dokud neni pripojena seriova linka
    }

  Serial.println("Setup can..");
  while(CAN.begin(CAN_500KBPS, MCP_8MHz) != CAN_OK){
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nCAN init ok!!\r\n");
  
  Timer1.initialize(50000);
  Timer1.attachInterrupt(timer_overflow);

  Serial.println("Inicializace -- OK");

}
void loop(){
  if (timmer_flag == true) {
   celk_voltage = voltage_measure (voltage_pin);
   timmer_flag = false;
   voltage_message();
   CAN.sendMsgBuf(0x00 + can_adress_current, 0, 4, buf_voltage);
  }
}

