void Motor(){
  if (Motorflag == 1) {
    for (int i=0;i<2;i++){
      CAN.sendMsgBuf(0x00000100+CANvescID[i], 1, 4, currentON);
      //Serial.print("jede- ");
      
    }
    }else{
      for (int i=0;i<2;i++){
        CAN.sendMsgBuf(0x00000100+CANvescID[i], 1, 4, currentOFF);
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
      Motorflag = Motorflag * (-1);
      } else {
        Motorflag = -1;
      }
     LOG();
    }
  Motor();
  Timer3.start(200); // Calls every 200 ms
}

/*---------------------------Led indikace ----------------------------------------------------------------*/
void LEDIndicator ()
{
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
  Serial.print("Pocet sekund jizdy: ");
  Serial.println(Count);
  Serial.print ("Motorflag: ");
  Serial.println(Motorflag);
  Serial.print("STOP Button: ");
  Serial.println(STOpSTARt);
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

