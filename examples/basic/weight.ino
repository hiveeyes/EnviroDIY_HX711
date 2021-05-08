/*
 *
 * Basic demonstration program for reading a HX711 ADC using the
 * `EnviroDIY_ModularSensors` datalogger framework.
 *
 * Derived from https://github.com/EnviroDIY/ModularSensors/blob/v0.28.4/examples/single_sensor/single_sensor.ino.
 *
 */
#include <Arduino.h>
#include <AviaSemiHX711.h>

const int32_t serialBaud = 115200;  // Baud rate for debugging

// Configure HX711 pins
#define HX711_PIN_DOUT  14 // DT
#define HX711_PIN_PDSCK 12 // SCK

// This is the raw sensor value for "0 kg".
// Write down the sensor value of the scale sensor with no load and adjust it here.
#define LOADCELL_ZERO_OFFSET    38623.0f

// This is the raw sensor value for a load weighing exactly 1 kg.
// Add a load with known weight in kg to the scale sensor, note the
// sensor value, calculate the value for a 1 kg load and adjust it here.
#define LOADCELL_KG_DIVIDER     22053


// Create a HX711 sensor object.
AviaSemiHX711 hx711(HX711_PIN_PDSCK, HX711_PIN_DOUT, LOADCELL_ZERO_OFFSET, LOADCELL_KG_DIVIDER);

// Create a new instance of the weight variable.
AviaSemiHX711_Weight hx711_weight(&hx711, "66666666-abcd-1234-ef00-1234567890ab");


void setup ()
{
    Serial.begin(serialBaud);
    Serial.print(F("Using ModularSensors Library version "));
    Serial.println(MODULAR_SENSORS_VERSION);

    Serial.println("Setting up HX711 sensor");
    hx711.setup();
}

void loop ()
{

    // Send power to the sensor and wake it up.
    Serial.println("Waking up HX711 sensor");
    hx711.powerUp();
    hx711.wake();

    // Read sensor.
    Serial.println("Reading HX711 sensor");
    hx711.update();

    // Display reading.
    Serial.print("Weight: ");
    Serial.print(hx711_weight.getValueString());
    Serial.println(" kg");

    // Put the sensor back to sleep
    Serial.println("Hibernating HX711 sensor");
    hx711.sleep();

    // Cut the sensor power
    hx711.powerDown();

    delay(1000);
}
