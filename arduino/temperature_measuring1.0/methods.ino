/*-----------------------Serial-initialization--------------------------------*/
void serial_initial(){
  Serial.begin(9600); 
  while (!Serial) {
    ; // ceka dokud neni pripojena seriova linka
    }
}

/*-----------------------Sensors-initialization--------------------------------*/
void sensors_initial(){
  sensor_1.begin(); // temperature
}

/*-----------------------Sensors-requesting-temperature--------------------------------*/
void sensors_request(){
  sensor_1.requestTemperatures();
}

/*-----------------------Sensors-get-temperature--------------------------------*/
void sensors_get_temp(){
  temp_c[0]=sensor_1.getTempCByIndex(0);
}

/*-----------------------Serial-print-temperature--------------------------------*/
void serial_print_temp(){
  Serial.print(temp_c[0],2);
  Serial.println(",");
}
