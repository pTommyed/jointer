void CANmsgToSerial(){
      Serial.println("-----------------------------------------");
      Serial.print("can address: ");
      Serial.print(CANmessageID);
      Serial.print("; lenght: ");
      Serial.println(len);
      for (int i=0;i<len;i++){
        Serial.print(buf[i]);
        Serial.print("\t");
      }
      Serial.println("");
      Serial.println("-----------------------------------------");
}

/*------------------------- Clear Buffer ------------------------------------------------------------------------*/

void ClearBuffer ()
{
  for (int i=0; i <= 7; i++) {
      buf[i] = 0;
  }
}

/*--------------------------- Přijatá zpráva CAN----------------------------------------------------------------------------------*/

void CAN_MESSAGE_ReceiveD(){
  int can_adress;
  if (CANmessageID > 2304){
    can_adress = (CANmessageID - 2304)+144;
    CAN.sendMsgBuf(0x0+can_adress, 0, 8, buf);
  }else {
    if (STOpSTARt == false){
      if (CANmessageID < 60){
          if (CANmessageID > 48){
            can_adress = (CANmessageID - 48)+768;
            CAN.sendMsgBuf(0x00000+can_adress, 1, 4, buf);
          } else if (CANmessageID > 32) {
              can_adress = (CANmessageID - 32)+512;
              CAN.sendMsgBuf(0x00000+can_adress, 1, 4, buf);
            } else if (CANmessageID > 16) {
                  can_adress = (CANmessageID - 16)+256;
                  CAN.sendMsgBuf(0x00000+can_adress, 1, 4, buf);
              }
      }    
    }
  }
  ClearBuffer();
}


