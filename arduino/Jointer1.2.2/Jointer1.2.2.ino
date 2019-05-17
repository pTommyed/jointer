
/*Definitivní verze - stejný proud na oba motory*/

#include <mcp_can.h>
#include <DueTimer.h>

const int ledPin =  A0;
const int Syrena = 8;


MCP_CAN CAN(2);

int           Motorflag = 1;     // kdyz je 1 tak motor jede
int           CANmessageID;
unsigned char len = 0;
byte buf[8];

int CANvescID[3] = {1,2,3};  // ID jednotlivých motorů

byte currentON[4]={0,0,10,0}; // tohle chce reverzovat, ma to znamenat 768mA pokud se tato hodnota změní musí se přepsat i current
byte currentOFF[4]={0,0,0,0}; // tohle chce reverzovat, ma to znamenat 0mA
byte currentREVERS[4]={0,0,250,0}; // brzdící proud
byte currentSTART[4]={0,0,31,64}; //  start proud
byte currentDIFERENCE[4]={0,0,0,0}; // proud který pro snižování diference otáček mezi koly
int current = 768; //mA

int STOpSTARt = 1; //pokud je nastaven na -1 je stop
byte StopButton = 24;
bool digitRead = false;

int Count = 0; // pocet opakovani smycky
int MaxPocetOpak = 2;
int Cas = 0;

bool Timmer4End = false;

unsigned long RPM[3] = {0,0,0}; // RPM pro jednotlivé motory
int RPMMAX = 400; // maximánlní povolené otáčky za minutu
int RPMMIN = 390; // minimánlní povolené otáčky za minutu
int RPMStart = 50; // otačky při startu, do kterých se má do motorů pouštět startovací proud
bool RPMMAXFlag = false; // značka pro regulaci otáček, že se přesáhla maximální mez
bool RPMMINFlag = false; // značka pro regulaci otáček, že se přesáhla minimální mez

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

  LOG();
  
  while(Timmer4End != true){
    delay(100);
  }

  Timer3.start(50); // Calls every 20 ms
}


void loop() {
  if (Timer3Over = true){
    Timer3.stop();
    ButtonRead();
    Serial.println(STOpSTARt);
    if (STOpSTARt == 1){
      Motorflag = 1;
    } else {
        Motorflag = -1;
      }
       //LOG();
    Motor();
    Timer3Over = false;
    ClearBuffer();
    Timer3.start(50); // Calls every 20 ms
  }
  /*if(CAN_MSGAVAIL == CAN.checkReceive()) {  
    CAN.readMsgBuf(&len, buf);
    CANmessageID = CAN.getCanId();
    CAN_MSG_Recev();
    //CANmsgToSerial();
    ClearBuffer();
    CANmessageID = -1;
  }*/
}

