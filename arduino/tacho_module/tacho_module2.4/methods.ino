/*--------------------------- Přijatá zpráva CAN----------------------------------------------------------------------------------*/

void CAN_MESSAGE_ReceiveD(){
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
}
/*-------------------------- Probuzeni arduina od interniho preruseni ----------------*/

void TimerInterrupt () {
  Timer3Over = true;
}

/*-------------------------- Sending request via CAN to vesc for tacho -------------------------*/

void sending_tacho_request() {
  int vesc_id;
  
  for (int i=2049;i<2050;i++){  //2053
    vesc_id = i-2049+1;
    request_buf[0] = vesc_id;
    CAN.sendMsgBuf(0x00000+i, 1, 3, request_buf);
  }
}
