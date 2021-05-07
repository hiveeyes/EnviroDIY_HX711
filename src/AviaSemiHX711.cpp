/**
 * @file AviaSemiHX711.cpp
 * @copyright 2021 Hiveeyes Labs
 * @author Andreas Motl <andreas@hiveeyes.org>
 *
 * @brief Implements the AviaSemiHX711 class.
 */

#include "AviaSemiHX711.h"


// The constructors
AviaSemiHX711::AviaSemiHX711(uint8_t doutPin, uint8_t sckPin,
                             long loadCellOffset, long loadCellDivider)
        : Sensor("AviaSemiHX711", HX711_NUM_VARIABLES, HX711_WARM_UP_TIME_MS,
                 HX711_STABILIZATION_TIME_MS, HX711_MEASUREMENT_TIME_MS,
                 -1, -1, 1) {
    _doutPin = doutPin;
    _sckPin = sckPin;
    _loadCellOffset = loadCellOffset;
    _loadCellDivider = loadCellDivider;

    HX711 loadcell;
    hx711_internal = loadcell;
}
// Destructor
AviaSemiHX711::~AviaSemiHX711() {}


String AviaSemiHX711::getSensorLocation(void) {
    String address = F("HX711_");
    address += String(_doutPin);
    address += String(_sckPin);
    return address;
}


// This turns on sensor power
void AviaSemiHX711::powerUp(void) {
    hx711_internal.power_up();
    // Set the status bit for sensor power attempt (bit 1) and success (bit 2)
    _sensorStatus |= 0b00000110;
}


// This turns off sensor power
void AviaSemiHX711::powerDown(void) {
    hx711_internal.power_down();
    // Unset the status bits for sensor power (bits 1 & 2),
    // activation (bits 3 & 4), and measurement request (bits 5 & 6)
    _sensorStatus &= 0b10000001;
}


bool AviaSemiHX711::setup(void) {
    bool retVal =
            Sensor::setup();  // this will set pin modes and the setup status bit

    // This sensor needs power for setup!
    bool wasOn = checkPowerOn();
    if (!wasOn) { powerUp(); }
    waitForWarmUp();

    // TODO: Check if we can somehow determine success or failure here.
    hx711_internal.begin(_doutPin, _sckPin);
    hx711_internal.set_offset(_loadCellOffset);
    hx711_internal.set_scale(_loadCellDivider);

    // Turn the power back off if it had been turned on
    if (!wasOn) { powerDown(); }

    retVal = true;
    return retVal;
}


bool AviaSemiHX711::addSingleMeasurementResult(void) {
    bool success = false;

    // Initialize float variables
    float weight = -9999;

    // Report
    MS_DBG(getSensorNameAndLocation(), F("is reporting:"));

    // Run measurement
    // TODO: Run multiple readings through "median" function.
    // TODO: Check if we can somehow determine success or failure here.
    weight = hx711_internal.get_units(10);

    // Sanity checks. Currently from BME280.
    /*
    // Assume that if all three are 0, really a failed response
    // May also return a very negative temp when receiving a bad response
    if ((temp == 0 && press == 0 && humid == 0) || temp < -40) {
        MS_DBG(F("All values 0 or bad, assuming sensor non-response!"));
        weight  = -9999;
    } else {
        success = true;
    }
    */

    // Report outcome
    MS_DBG(F("  Weight:"), weight, F(" kg"));

    // Bad measurement / Empty result
    //MS_DBG(getSensorNameAndLocation(), F("is not currently measuring!"));

    verifyAndAddMeasurementResult(HX711_WEIGHT_VAR_NUM, weight);

    // Unset the time stamp for the beginning of this measurement
    _millisMeasurementRequested = 0;
    // Unset the status bits for a measurement request (bits 5 & 6)
    _sensorStatus &= 0b10011111;

    return success;
}
