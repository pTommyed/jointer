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
    STOpSTARt = true;
  } else {
    STOpSTARt = false;
  }
}

/*---------------------------- Zašle stav tlačítek apu--------------------------------------------------------------------------*/

void SendButtonStatusAPU() {
  byte messageStopButt[1];
   if (STOpSTARt == false){
      messageStopButt[0]=0;
      CAN.sendMsgBuf(0x1, 0, 1, messageStopButt);  // pokud je messageStopButt=0 Stop tlačítko není zmáčknuté
    } else {
        messageStopButt[0]=1;
        CAN.sendMsgBuf(0x1, 0, 1, messageStopButt);  // pokud je messageStopButt=1 Stop tlačítko není zmáčknuté
      }
}
/*--------------------------- Inicializace Tabulky Can zprav ---------------------------------------------------------------------*/
void can_message_init(){
  for ( int can_adress = 0;can_adress<2309;can_adress++){
    if (can_adress > 2304){
      can_adress_tabel[can_adress] = (can_adress - 2304)+144;
    }else {
      if (can_adress < 60){
          if (can_adress > 48){
            can_adress_tabel[can_adress] = (can_adress - 48)+768;
          } else if (can_adress > 32) {
              can_adress_tabel[can_adress] = (can_adress - 32)+512;
            } else if (can_adress > 16) {
                  can_adress_tabel[can_adress] = (can_adress - 16)+256;
              }
      }    
     }
  }
}
/*--------------------------- Přijatá zpráva CAN----------------------------------------------------------------------------------*/

void CAN_MESSAGE_ReceiveD(){
  int can_adress;
  if (CANmessageID > 2304){
    CAN.sendMsgBuf(0x0+can_adress_tabel[CANmessageID], 0, 8, buf);
  }else {
    if (STOpSTARt == false){
      CAN.sendMsgBuf(0x00000+can_adress_tabel[CANmessageID], 1, 4, buf);    
    }
  }
  ClearBuffer();
}
/*--------------------------- Stop Motors------------------------------------------------------------------------------------------------------*/

void STOP(){
  for (int i=0;i<4;i++){
     CAN.sendMsgBuf(0x00000200+CANvescID[i], 1, 4, currentREVERS); // motorum se pošle brzdí proud
     CAN.sendMsgBuf(0x0020+CANvescID[i], 0, 4, currentREVERS);
   }
}
/*---------------------------- Cycle count --------------------------------------------------------------------------------------------------*/

void CountCycle(){
  byte bufCount[1];

  bufCount[0]= CycleCount;
  CAN.sendMsgBuf(0x02, 0, 1, bufCount);
  if (CycleCount==255){
    CycleCount = 0;
  } else {
     CycleCount = CycleCount+1;
    }
  
}
