#include <mcp_can.h>
#include <DueTimer.h>

int CANvescID[4] = {1,2,3,4};  // ID jednotlivých motorů

MCP_CAN CAN(2);

int  CANmessageID;
unsigned char len = 0;
byte buf[8];

void setup() {
 
  Serial.begin(250000);

  Serial.println("Setup can..");
  while(CAN.begin(CAN_500KBPS, MCP_8MHz) != CAN_OK){
    digitalWrite(stop_led, HIGH);
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nCAN init ok!!\r\n");
   
   // pro jine verye aruduino }jinak nepreklada]  
  CAN.init_Mask(0, 1, 0x06FF0000);
  CAN.init_Filt(0, 1, 0x00000501);
  CAN.init_Filt(1, 1, 0x00000502);
  CAN.init_Filt(2, 1, 0x00000503);
  CAN.init_Filt(3, 1, 0x00000504);
  }
}

void loop() {
  if(CAN_MSGAVAIL == CAN.checkReceive()) {  
    CAN.readMsgBuf(&len, buf);
    CANmessageID = CAN.getCanId();
    //Serial.println("in loop");
    //Serial.println(CANmessageID);
    //CANmsgToSerial();
    CAN_MESSAGE_ReceiveD();
  }
}
