
/*Definitivní verze -různý proud pro motory*/

#include <mcp_can.h>
#include <DueTimer.h>

const int ledPin =  A0;
const int Syrena = 8;


MCP_CAN CAN(2);

int           CANmessageID;
unsigned char len = 0;
byte buf[8];

int CANvescID[2] = {1,2};  // ID jednotlivých motorů

int CANAdressReceiveTable[2][2]={{2305,91}, //{přijatá hodntoa,přeposílaná hodntota} -- to co se má jen přeposílat APU
                                 {2306,92}}; 
int CANAdressSendTable[4][2]={{11,101},//{přijatá hodntoa,přeposílaná hodntota} -- to co se má posílat vESCum
                              {12,102},
                              {21,201},
                              {22,202}}; 

byte currentOFF[4]={0,0,0,0}; // tohle chce reverzovat, ma to znamenat 0mA
byte currentREVERS[4]={0,0,250,0}; // brzdící proud  

int STOpSTARt = 1; //pokud je nastaven na -1 je stop
byte StopButton = 24;
bool digitRead = false;

bool Timmer4End = false;

bool Timer3Over = false; //indikace přetečení timeru3

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
  
  while(Timmer4End != true){
    delay(100);
  }

  Timer3.start(50); // Calls every 50 ms
}


void loop() {
  if (Timer3Over = true){
    Timer3.stop();
    ButtonRead();
    //Serial.println(STOpSTARt);
    Motor();
    SendButtonStatusAPU();
    Timer3Over = false;
    Timer3.start(50); // Calls every 50 ms
  }
  if(CAN_MSGAVAIL == CAN.checkReceive()) {  
    CAN.readMsgBuf(&len, buf);
    CANmessageID = CAN.getCanId();
    //CANmsgToSerial();
    CAN_MESSAGE_ReceiveD();
    ClearBuffer();
  }
}

