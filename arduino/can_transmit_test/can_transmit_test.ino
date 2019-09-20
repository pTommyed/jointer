#include <mcp_can.h>

MCP_CAN CAN(2);

void setup() {
  Serial.begin(250000);

  Serial.println("Setup can..");
  while(CAN.begin(CAN_500KBPS, MCP_8MHz) != CAN_OK){
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nCAN init ok!!\r\n");

}

void loop() {
  can_message_send ();
}


void can_message_send () {
  
  for (int i=10;i<19;i++){
     byte buf[i-9];
     for (int j=0;j<(i-9);i++){
      buf[j]=i-9;
     }
     CAN.sendMsgBuf(0x0+i, 0, (i-9), buf);
  }
}
