#include <TimerOne.h>
#include <mcp_can.h>

float celk_proud; // proud v obvodu se zatezi

const int led = 8;

MCP_CAN CAN(10);
int           CANmessageID;
unsigned char len = 0;
byte buf[3]={0,0,0};
int can_adress = 16;

const int current_pin = A0;

bool Timmer1End = false;

bool timmer_flag = false;


void setup(){
  Serial.begin(250000); 
  while (!Serial) {
    ; // ceka dokud neni pripojena seriova linka
    }

  pinMode(led, OUTPUT);

  Serial.println("Setup can..");
  while(CAN.begin(CAN_500KBPS, MCP_8MHz) != CAN_OK){
    digitalWrite(led,HIGH);
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nCAN init ok!!\r\n");

  Timer1.attachInterrupt(BeepDown);
  
  BeepUP ();
  
  while(Timmer1End != true){
    delay(100);
  }
  digitalWrite(led, LOW);
  
  Timer1.initialize(50000);
  Timer1.attachInterrupt(timer_overflow);

  Serial.println("Inicializace -- OK");

}
void loop(){
  if (timmer_flag == true) {
   celk_proud = proud20A (current_pin);
   timmer_flag = false;
   create_message();
   CAN.sendMsgBuf(0x00 + can_adress, 0, 3, buf);
   //Serial.print("Current = ");
   //Serial.println(celk_proud,3);
  }
}

