void Motor(){
  /*Serial.print(RPM[0]);
  Serial.print(";");
  Serial.println(RPM[1]);*/
  if (STOpSTARt == -1) { // pokud je STOpSTAR ==-1 takje stisklé Stop Button
     for (int i=0;i<2;i++){
        CAN.sendMsgBuf(0x00000200+CANvescID[i], 1, 4, currentREVERS); // motorum se pošle brzdí proud
        CAN.sendMsgBuf(0x0020+CANvescID[i], 0, 4, currentREVERS);
     }
  }
}

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

/*-------------------------- Probuzeni arduina od interniho preruseni ----------------*/

void TimerInterrupt () {
  Timer3Over = true;
}

/*---------------------------Led indikace ----------------------------------------------------------------*/
void LEDIndicator () {
  for (int i=0; i <= 3; i++) {
      digitalWrite(ledPin, HIGH);
      delay(200);
      digitalWrite(ledPin, LOW);
      delay(200);
  }
}

/*------------------------- Clear Buffer ------------------------------------------------------------------------*/

void ClearBuffer ()
{
  for (int i=0; i <= 7; i++) {
      buf[i] = 0;
  }
}

/*-------------------------- SyrenaDown ------------------------------------------------------------------------------*/

void BeepDown () {
  
  Timer4.stop();
  digitalWrite(Syrena, LOW);
  digitalWrite(ledPin, LOW);
  Timmer4End = true;
}

/*-------------------------- SyrenaUP ------------------------------------------------------------------------------*/

void BeepUP () {
  
  digitalWrite(Syrena, HIGH);
  digitalWrite(ledPin, HIGH);
  Timer4.start(300000);
}

/*---------------------------StopButton--------------------------------------------------------------------------------------*/

void ButtonRead(){
  digitRead = digitalRead(StopButton);
  if (digitRead==true){
    STOpSTARt = -1;
  } else {
    STOpSTARt = 1;
  }
}

/*---------------------------- Zašle stav tlačítek apu--------------------------------------------------------------------------*/

void SendButtonStatusAPU() {
  byte messageStopButt[1];
   if (STOpSTARt == 1){
      messageStopButt[0]=0;
      CAN.sendMsgBuf(0x1, 0, 1, messageStopButt);  // pokud je messageStopButt=0 Stop tlačítko není zmáčknuté
    } else {
        messageStopButt[0]=1;
        CAN.sendMsgBuf(0x1, 0, 1, messageStopButt);  // pokud je messageStopButt=1 Stop tlačítko není zmáčknuté
      }
}

/*--------------------------- Přijatá zpráva CAN----------------------------------------------------------------------------------*/

void CAN_MESSAGE_ReceiveD(){
  if (CANmessageID > 2304){
    for (int i=0;i<2;i++){
      if (CANmessageID == CANAdressReceiveTable[i][0]){
        CAN.sendMsgBuf(0x0+CANAdressReceiveTable[i][1], 0, 8, buf);
      }
    }
  }else {
    if (STOpSTARt == 1){
      for (int i=0;i<4;i++){
        if (CANmessageID == CANAdressSendTable[i][0]){
          CAN.sendMsgBuf(0x00000+CANAdressSendTable[i][1], 1, 4, buf);
        }
      }
    }
  }
}
