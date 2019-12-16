//####################################################
//#    KLOUBAK: pinout_led_tetser
//#    Copyright (c) 2019 ROBOTIKA
//#    programmed by: Tomas Picha
//####################################################

// DESCRTIPTION: - code for testing of led pinout 
// HW: Aruino Nano, buzzer, 8xled indication, MCP2515+TJA1050 CAN module

/*----------------------- DEFINITION -----------------------------------*/
const byte pinout_count = 14;

byte pinout[pinout_count]; 

/*----------------------- SETUP ----------------------------------------*/
void setup() {
  serial_initial();
  pinout_initial();
  Serial.println(" - initialization successfully done!");
}

/*---------------------- MAIN LOOP -------------------------------------*/
void loop() {
  pinout_testing();
}
