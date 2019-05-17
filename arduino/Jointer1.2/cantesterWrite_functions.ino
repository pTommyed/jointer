void Motor(){
  //Serial.print("RPM[3]: ");
  Serial.print(RPM[3]);
   Serial.println(pomSerialRead);
  pomSerialRead=0;
  if (Motorflag == 1) {
    for (int i=0;i<3;i++){
      if (RPM[CANvescID[i]]< RPMStart) {
        CAN.sendMsgBuf(0x00000100+CANvescID[i], 1, 4, currentSTART);
      } else if (RPM[CANvescID[i]]<RPMMIN){
          CAN.sendMsgBuf(0x00000100+CANvescID[i], 1, 4, currentON);
          //DIFRPMCount(); //pokud budeme chtít regulovat rozdíly otáček na kolech pak se musí zakomentovat předešlý řádek
          RPMMAXFlag = false;
          RPMMINFlag = true;
        } else if (RPM[CANvescID[i]]>RPMMAX) {
            CAN.sendMsgBuf(0x00000100+CANvescID[i], 1, 4, currentOFF);
            RPMMAXFlag = true;
            RPMMINFlag = false;
          } else if (RPM[CANvescID[i]]<=RPMMAX && RPM[CANvescID[i]]>=RPMMIN) {
              if (RPMMAXFlag == true){
                 CAN.sendMsgBuf(0x00000100+CANvescID[i], 1, 4, currentOFF); 
                } else if (RPMMINFlag == true){
                    CAN.sendMsgBuf(0x00000100+CANvescID[i], 1, 4, currentON);
                    //DIFRPMCount(); //pokud budeme chtít regulovat rozdíly otáček na kolech pak se musí zakomentovat předešlý řádek
                  }  
               } 
      //Serial.print("jede- ");
      }
    }else{
      for (int i=0;i<3;i++){
        if (RPM[CANvescID[i]]>0) {
          CAN.sendMsgBuf(0x00000200+CANvescID[i], 1, 4, currentREVERS);
        }else {
          CAN.sendMsgBuf(0x00000100+CANvescID[i], 1, 4, currentOFF);
        }
        //Serial.println("nejede-");
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

/*--------------------------CAN-MSG-Recev---------------------------------------------*/

void CAN_MSG_Recev() {
  int VescID = -1;
  
  if (CANmessageID == 2305){ // zkontrolovat správnost adresy
    VescID = 1;
  }
  if (CANmessageID == 2306){ // zkontrolovat správnost adresy
    VescID = 2;
  }
  if (CANmessageID == 2307){ // zkontrolovat správnost adresy
    VescID = 3;
  }
  CAN_MSG_RPM(VescID);
}

/*-------------------------- CAN-MSG-RPM----------------------------------------------*/

void CAN_MSG_RPM(int VescID) {
  int RPMpom[4]={0,0,0,0};
  
  RPM[VescID] = 0;
  for (int i=0;i<5;i++) {
    RPMpom[i] = buf[i] * pow(256,(3-i)) ;
    //Serial.println(RPMpom[i]);
  }
  for (int i=0;i<5;i++) {
    RPM[VescID] = RPM[VescID] + RPMpom[i];
  }
  RPM[VescID]= RPM[VescID]/3;
}

/*-------------------------- Probuzeni arduina od interniho preruseni ----------------*/

void TimerInterrupt () {
  Timer3.stop();
  Cas = Cas +1;
  ButtonRead();
  if (STOpSTARt == -1){
    Motorflag = -1;
  }
  if(Cas==5000) {
    Count = Count +1;
    Cas = 0;
    if (Count < MaxPocetOpak && STOpSTARt == 1){
      Motorflag = 1;
      } else {
        Motorflag = -1;
      }
     //LOG();
    }
  Motor();
  Timer3.start(150); // Calls every 20 ms
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

/*----------------------------LOG------------------------------------------------------------------------------------------*/

void LOG() {
  Serial.print("Count: ");
  Serial.println(Count);
  Serial.print ("Motorflag: ");
  Serial.println(Motorflag);
  Serial.print("STOP Button: ");
  Serial.println(STOpSTARt);
  Serial.print("RPM[1]: ");
  Serial.println(RPM[1]);
  Serial.print("RPM[2]: ");
  Serial.println(RPM[2]);
  Serial.print("RPM[3]: ");
  Serial.println(RPM[3]);
  Serial.println("-----------------------------------------");
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

/*-------------------------DifferenceRPM--------------------------------------------------------------------------------------*/

void DIFRPMCount() {
  int RPMDifference = 0;
  float RPMDifferencePom = 0.0;
  
  if ((RPM[CANvescID[0]] - RPM[CANvescID[1]])>0) {
    RPMDifferencePom = (RPM[CANvescID[0]] / RPM[CANvescID[1]]);  // *10
    RPMDifference = RPMDifferencePom / 1;
    current = current * RPMDifference;
    CurrentCreateMSG();
    CAN.sendMsgBuf(0x00000100+CANvescID[1], 1, 4, currentDIFERENCE);
  } else {
      RPMDifferencePom = (RPM[CANvescID[1]] / RPM[CANvescID[0]]);  // *10
      RPMDifference = RPMDifferencePom / 1;
      current = current * RPMDifference;
      CurrentCreateMSG();
      CAN.sendMsgBuf(0x00000100+CANvescID[0], 1, 4, currentDIFERENCE);
    }
}

/*----------------------createCanCurrentMessage------------------------------------------------------------------------------*/

void CurrentCreateMSG(){
  int currentpom = 0;
  
  for (int i=0;i<5;i++) {
   currentDIFERENCE[i] = current / pow(256,(3-i)) ;
  }
}

