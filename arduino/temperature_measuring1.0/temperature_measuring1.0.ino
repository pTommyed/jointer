#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS_1 2

OneWire oneWire_BUS1(ONE_WIRE_BUS_1);

DallasTemperature sensor_1(&oneWire_BUS1);

float temp_c[1] = {0}; // temperature in °C

void setup()
{
    serial_initial();
    sensors_initial();
}

void loop (){
  sensors_request();
  sensors_get_temp();
  serial_print_temp();
}

