//####################################################
//#    Battery_charger: Battery_charger1.1
//#    Copyright (c) 2019 ROBOTIKA
//#    programmed by: Tomas Picha, Jan Kaderabek
//####################################################


// DESCRTIPTION: -   

// HW: Aruino nano, current probe ACS712 20 A, relay

/*----------------------- DEPENDENCES ----------------------------------*/
#include <TimerOne.h>
#include <stdint.h> //due to wdt
#include <avr/wdt.h> // wdt lib
#include <TM74HC595Display.h> // 4bitov display

/*----------------------- DEFINITION -----------------------------------*/
byte voltage_pin = A1;
byte current_pin = A0;

byte relay_pin = 4; 
byte buzzer_pin = 5; 

byte SCLK = 7; //display
byte RCLK = 6;  //dispaly
byte DIO = 8; // display

TM74HC595Display disp(SCLK, RCLK, DIO); //display
unsigned char LED_0F[29]; //display

float r1 = 680300.0;  //value of resistance of resistor in voltage divider
float r2 = 68100.0; //value of resistance of resistor in voltage divider

float voltage = 0;
float voltage_max_limit = 42.1; //V

float current = 0;
float current_max_limit =4.2; //A
//float current_min_limit =2.0; //A 2.5 A

bool charged = false;

int preload_timer = 62411;// preload timer 65536-16MHz/256/5Hz --- 256 is prescaler; 65563 - 16 bit counter(Timer1); 20Hz - requiered frequency --- 62411 ; 10 hz = 59286; 5hz =53036 ; 2hz=34286

/*----------------------- SETUP ----------------------------------------*/
void setup() {
   pinouts_initial();
   serial_initial();
   characters_table_for_display();
   display_init();
   timer_init();
   charging();
   wdt_enable(WDTO_4S);  //wdt reset 60ms ; 120MS ; 250MS ; 500MS ; 1S ; 2S ; 4S ; 8s ;
   Serial.println("initialization successfully done!");
}

/*----------------------- LOOP ------------------------------------------*/
void loop() {
  display_voltage();
  display_current();
  write_to_serial();
}
