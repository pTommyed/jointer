#include <mcp_can.h>
#include <DueTimer.h>

#define fifo_buff_size 256

static volatile int fifo_buff[fifo_buff_size];

byte fifo_head = 0;
byte fifo_tail = 0;
const byte rx_can_interrupt_pin = 3;

int CANvescID[4] = {1,2,3,4};  // ID jednotlivých motorů

MCP_CAN CAN(2);

int can_adress_transmit = 131; // can id odchozí zprávy 0X83
int  CANmessageID;
unsigned char len = 0;
byte tacho_buf[8];
byte request_buf[3] = {0,0,4}; 

int tacho_bufer_index_tabel[4]={0,2,4,6};

bool Timer3Over = false; //indikace přetečení timeru3

void setup() {
 
  Serial.begin(250000);

  pinMode(rx_can_interrupt_pin,INPUT);

  Serial.println("Setup can..");
  while(CAN.begin(CAN_500KBPS, MCP_8MHz) != CAN_OK){
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nCAN init ok!!\r\n");
   
   // pro jine verye aruduino }jinak nepreklada]  
  CAN.init_Mask(0, 1, 0x00000600); //CAN.init_Mask(0, 1, 0x06FF0000);
  CAN.init_Mask(1, 1, 0x00000600);
  CAN.init_Filt(0, 1, 0x00000501);
  CAN.init_Filt(1, 1, 0x00000502);
  CAN.init_Filt(2, 1, 0x00000503);
  CAN.init_Filt(3, 1, 0x00000504);

  Timer3.attachInterrupt(TimerInterrupt);

  //Timer3.start(100000); // Calls every 100 ms

  attachInterrupt(rx_can_interrupt_pin, rx_can_interrupt, FALLING);

  Serial.println("Inicializace OK");
}

void loop() {
  if (fifo_head!=fifo_tail) {
    rx_fifo_reading ();
  }
  /*if (Timer3Over == true){
    CAN.sendMsgBuf(0x0+can_adress_transmit, 0, 8, tacho_buf); // sending tacho infos to APU
    sending_tacho_request(); // sending request to vescs
    Timer3Over = false;
  }*/
  
  /*if(CAN_MSGAVAIL == CAN.checkReceive()) {  
    CAN.readMsgBuf(&len, buf);
    CANmessageID = CAN.getCanId();
    Serial.println(CANmessageID);
    CANmsgToSerial();
    CAN_MESSAGE_ReceiveD();
  }
  //Serial.println("in loop");*/
}
