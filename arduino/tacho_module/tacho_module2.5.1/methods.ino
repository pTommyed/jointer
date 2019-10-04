/*--------------------------- Přijatá zpráva CAN----------------------------------------------------------------------------------*/

void CAN_MESSAGE_ReceiveD(int vesc_id){
  sending_tacho_request(vesc_id); // sending request to vescs
  can_message_processing(vesc_id);  
}
/*-------------------------- Probuzeni arduina od interniho preruseni ----------------*/

void TimerInterrupt () {
  Timer3Over = true;
}

/*-------------------------- can message processing -------------------------*/

void can_message_processing(int vesc_id) {
  int i;
  int tacho;
  
    for (i=0; i<10;i++) {
      while((CAN.readMsgBuf(&len, buf))==CAN_NOMSG){
        if (Timer3Over == true){
          break;
        }
      }
      if (buf[0]==42){  
        Sending_tacho_buf_apu(vesc_id);
      }
      if (Timer3Over == true){
          break;
      }
    }
   CAN.sendMsgBuf(0x0+can_adress_transmit, 0, 8, tacho_buf); // sending tacho infos to APU  
}

/*-------------------------- Sending request via CAN to vesc for tacho -------------------------*/

void sending_tacho_request(int vesc_id) {
  int can_id = 2048;
  
  request_buf[0] = CANvescID[vesc_id]; // jeden vesc4x --- CANvescID[0]
  //Serial.println(request_buf[0]);
  CAN.sendMsgBuf(0x00000+can_id+CANvescID[vesc_id], 1, 3, request_buf); // jeden vesc 4x nahradit ----- CAN.sendMsgBuf(0x00000+can_id+CANvescID[0], 1, 3, request_buf);
}

/*-------------------------- Sending tacho_buf to apu -------------------------*/

void Sending_tacho_buf_apu(int buf_position) {
  int i = 7;
  buf_position = (buf_position * 2) + 1;
  
  for (int j=0;j<2;j++){
    i=i-j;
    //Serial.print(buf[i]);
    tacho_buf[buf_position - j] =buf[i];
  }
  //Serial.println("");
}
