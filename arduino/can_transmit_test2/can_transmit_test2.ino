#include <mcp_can.h>
#include <SPI.h>


MCP_CAN CAN(10);

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
  
    for (int i=1;i<9;i++){
     byte buf[i];
     for(int j=0;j<8;j++){
      buf[j]=j;
      //Serial.print(buf[j]);
     }
     CAN.sendMsgBuf(0x0+(i-1), 0, i, buf);
    }
}
