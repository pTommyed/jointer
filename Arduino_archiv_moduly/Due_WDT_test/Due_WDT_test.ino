#include <DueTimer.h>

#define WDT_KEY (0xA5) /*KEY: Password -- Should be written at value 0xA5. 
                         Writing any other value in this field aborts the write operation */

void watchdogSetup(void) {/*** watchdogDisable (); ***/}

bool Timer3Over = false;
bool blik= false;

byte led_pin = 5;


void setup() {
  serial_initial();
  pinMode(led_pin,OUTPUT);
  digitalWrite(led_pin,LOW);
  Timer3.attachInterrupt(TimerInterrupt);
  Serial.println("inicialuation - done!");
  wdt_initial();
  Timer3.start(50000); // Calls every 50 mikros (20 Hz)

}

void loop() {
  if (Timer3Over == true) {
    Timer3Over = false;
    wdt_reset();
    /*blik=!blik;
    if (blik==false){
      digitalWrite(led_pin,LOW);
    } else {
      digitalWrite(led_pin,HIGH);
    }*/
    uint32_t status = (RSTC->RSTC_SR & RSTC_SR_RSTTYP_Msk) >> 8; // Get status from the last Reset
    Serial.print("RSTTYP = 0b"); 
    Serial.println(status, BIN);  // Should be 0b010 after first watchdog reset
  }

}

/*-----------------------wdt initial--------------------------------*/
void wdt_initial(){
  // Enable watchdog.
  WDT->WDT_MR = WDT_MR_WDD(0xFF) // velikost musí pokrýt hodnotě v WDT_MR_WDV(xx)
                | WDT_MR_WDRPROC
                | WDT_MR_WDRSTEN
                | WDT_MR_WDV(33); //set wdt to 0,12890625‬ S
                /*| WDT_MR_WDV(256 * 2); // Watchdog triggers a reset after 2 seconds if underflow
                                       // 2 seconds equal 84000000 * 2 = 168000000 clock cycles
   Slow clock is running at 32.768 kHz
   watchdog frequency is therefore 32768 / 128 = 256 Hz
   WDV holds the periode in 256 th of seconds  */
}


/*-----------------------wdt reset--------------------------------*/
void wdt_reset(){
  //Restart watchdog
  WDT->WDT_CR = WDT_CR_KEY(WDT_KEY)
                | WDT_CR_WDRSTT;
}

/*-------------------------- Timer3 overflow ----------------*/

void TimerInterrupt () {
  Timer3Over = true;
}

/*-----------------------Serial initialization--------------------------------*/
void serial_initial(){
  Serial.begin(250000); 
  while (!Serial) {
    ; 
    }
}
