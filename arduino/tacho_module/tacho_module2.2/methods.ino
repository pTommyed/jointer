/*void CANmsgToSerial(){
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
}*/

/*------------------------- Clear Buffer ------------------------------------------------------------------------*/

/*void ClearBuffer ()
{
  for (int i=0; i <= 7; i++) {
      buf[i] = 0;
  }
}*/

/*--------------------------- Přijatá zpráva CAN----------------------------------------------------------------------------------*/

/*void CAN_MESSAGE_ReceiveD(){
  int vesc_number;
  int j = 6;
  
  if (CANmessageID > 1280 && CANmessageID < 1285){
    if (buf[0] == 42){
      vesc_number = CANmessageID - 1281;
      for (int i=tacho_bufer_index_tabel[vesc_number]; i<(tacho_bufer_index_tabel[vesc_number]+2);i++ ) {
        tacho_buf[i]= buf[j];
        j++; 
      }
    }
  }
  //ClearBuffer();
}
*/
/*-------------------------- Probuzeni arduina od interniho preruseni ----------------*/

void TimerInterrupt () {
  Timer3Over = true;
}

/*-------------------------- Sending request via CAN to vesc for tacho -------------------------*/

void sending_tacho_request() {
  int vesc_id;
  
  for (int i=2049;i<2053;i++){
    vesc_id = i-2049+1;
    request_buf[0] = vesc_id;
    CAN.sendMsgBuf(0x00000+i, 1, 3, request_buf);
  }
}

/*-------------------------- Detecting of incoming can message -------------------------*/

void rx_can_interrupt () {
  byte tmphead;
  CAN.readMsgBuf(&len, buf);
  tmphead = (fifo_head + 1) & fifo_buff_mask;
  fifo_buff[tmphead] = CAN.getCanId();
  tmphead = (tmphead + 1) & fifo_buff_mask;
  for (int i = 0; i < len; i++) {
      tmphead = (tmphead + i) & fifo_buff_mask;
      fifo_buff[tmphead] = buf[i];
    }
    fifo_head = tmphead;  //store new index  

    Serial.println(".");
}

/*-------------------------- reading of fifo buffer -------------------------*/

void rx_fifo_reading () { 
  int vesc_number;

  if (fifo_head>fifo_tail) {
    for (int i= fifo_tail;i<=fifo_head;i++){
      if (fifo_buff[i] > 1280 && fifo_buff[i] < 1285) {
        if (fifo_buff[i+1] == 42){
          for (int j=i+2;j<i+10;j++){
            Serial.print(fifo_buff[j]);
          }
          Serial.println("------------------");
          i=i+9;
        }
      }
    }
  } else {
      for (int i= fifo_tail;i<fifo_buff_size;i++){
      if (fifo_buff[i] > 1280 && fifo_buff[i] < 1285) {
        if (fifo_buff[i+1] == 42){
          for (int j=i+2;j<i+10;j++){
            Serial.print(fifo_buff[j]);
          }
          Serial.println("------------------");
          i=i+9;
        }
      }
    }
    for (int i= 0;i<=fifo_head;i++){
      if (fifo_buff[i] > 1280 && fifo_buff[i] < 1285) {
        if (fifo_buff[i+1] == 42){
          for (int j=i+2;j<i+10;j++){
            Serial.print(fifo_buff[j]);
          }
          Serial.println("------------------");
          i=i+9;
        }
      }
    }
  }
  fifo_tail = fifo_head;
}
