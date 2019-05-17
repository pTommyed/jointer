#include <mcp_can.h>
#include <DueTimer.h>

const int ledPin =  A0;
const int Syrena = 8;


MCP_CAN CAN(2);

int           Motorflag = 1;     // kdyz je 1 tak motor jede
int           CANmessageID;
unsigned char len = 0;
byte buf[8];

int CANvescID[2] = {1,2};  // ID jednotlivých motorů
byte currentON[4]={0,0,3,0}; // tohle chce reverzovat, ma to znamenat 768mA
byte currentOFF[4]={0,0,0,0}; // tohle chce reverzovat, ma to znamenat 0mA

int STOpSTARt = 1; //pokud je nastaven na -1 je stop
byte StopButton = 24;
bool digitRead = false;

int Count = 0; // pocet opakovani smycky
int MaxPocetOpak = 10;
int Cas = 0;

bool Timmer4End = false;

void setup() {
  Serial.begin(250000);

  pinMode(ledPin, OUTPUT);
  pinMode(Syrena,OUTPUT); //syrena
  
  digitalWrite(ledPin, LOW);
  digitalWrite(Syrena, LOW);

  pinMode(StopButton,INPUT_PULLUP);
   
  Serial.println("Setup can..");
  while(CAN.begin(CAN_500KBPS, MCP_8MHz) != CAN_OK){
    digitalWrite(ledPin, HIGH);
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nCAN init ok!!\r\n");

  LEDIndicator ();

  Timer3.attachInterrupt(TimerInterrupt);

  Timer4.attachInterrupt(BeepDown);

  BeepUP ();

  LOG();
  
  while(Timmer4End != true){
    delay(100);
  }

  Timer3.start(200); // Calls every 200 ms
}


void loop() {
  ClearBuffer();
  /*if(CAN_MSGAVAIL == CAN.checkReceive()) {  
    CAN.readMsgBuf(&len, buf);
    CANmessageID = CAN.getCanId();
    CANmsgToSerial();
  }*/
}

