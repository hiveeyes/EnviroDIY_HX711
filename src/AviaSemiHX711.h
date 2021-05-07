/**
 * @file AviaSemiHX711.h
 * @copyright 2021 Hiveeyes Labs
 * @author Andreas Motl <andreas@hiveeyes.org>
 *
 * @brief Contains the AviaSemiHX711 sensor subclass and the variable subclasses
 * AviaSemiHX711_Weight and AviaSemiHX711_Raw.
 *
 * An EnviroDIY sensor adapter for the canonical HX711 Arduino library by
 * Bogdan Necula. The Avia Semiconductor HX711 24-Bit Analog-to-Digital
 * Converter (ADC) is often used for reading weight scales.
 *
 * The sensor adapter depends on the [HX711 library](https://github.com/bogde/HX711).
 *
 * @copydetails AviaSemiHX711
 */
/* clang-format off */
/**
 * @defgroup sensor_hx711 HX711
 * Classes for the HX711 ADC.
 *
 * @ingroup the_sensors
 *
 * @tableofcontents
 * @m_footernavigation
 *
 * @section sensor_hx711_intro Introduction
 * > The HX711 is a 24-Bit Analog-to-Digital Converter (ADC) for weight scales.
 *
 * Communication with the chip is concluded with a custom protocol.
 * TODO: Add notes about power source, see BME280.
 *
 * The [Arduino HX711 library](https://github.com/bogde/HX711)
 * is used internally for communication with the HX711.
 *
 * @section sensor_hx711_datasheet Sensor Datasheet
 * Information about the sensor can be found at:
 *
 * - https://community.hiveeyes.org/t/hx711-adc/1711
 * - https://community.hiveeyes.org/t/bosche-hx0a-single-point-load-cell/2838
 *
 * [Datasheet](https://cdn.sparkfun.com/datasheets/Sensors/ForceFlex/hx711_english.pdf)
 *
 * @section sensor_hx711_ctor Sensor Constructors
 * {{ @ref AviaSemiHX711::AviaSemiHX711(uint8_t, uint8_t, long, long) }}
 *
 * ___
 * @section sensor_hx711_examples Example Code
 * The HX711 is used in the @menulink{hx711} example.
 *
 * @menusnip{hx711}
 */
/* clang-format on */

// Header Guards
#ifndef SRC_SENSORS_AVIASEMIHX711_H_
#define SRC_SENSORS_AVIASEMIHX711_H_

// Debugging Statement
// #define MS_AVIASEMIHX711_DEBUG

#ifdef MS_AVIASEMIHX711_DEBUG
#define MS_DEBUGGING_STD "AviaSemiHX711"
#endif

// Included Dependencies
#include "ModSensorDebugger.h"
#undef MS_DEBUGGING_STD
#include "VariableBase.h"
#include "SensorBase.h"
#include <HX711.h>

// Sensor Specific Defines
/** @ingroup sensor_hx711 */
/**@{*/

/// @brief Sensor::_numReturnedValues; the HX711 can report 2 values.
#define HX711_NUM_VARIABLES 2

/**
 * @brief Sensor::_readCount; Take X readings for producing a result.
 *
 * 10 for good numbers
 */
#define HX711_READ_COUNT 10
/**@}*/

/**
 * @anchor sensor_HX711_timing
 * @name Sensor Timing
 * The sensor timing for a HX711
 */
/**@{*/
/// @brief Sensor::_warmUpTime_ms; HX711 warms up in ???ms.
#define HX711_WARM_UP_TIME_MS 100
/**
 * @brief Sensor::_stabilizationTime_ms; HX711 is stable after ???ms.
 *
 */
#define HX711_STABILIZATION_TIME_MS 1000
/**
 * @brief Sensor::_measurementTime_ms; HX711 takes ???ms to complete a
 * measurement.
 *
 */
#define HX711_MEASUREMENT_TIME_MS 1100
/**@}*/

/**
 * @anchor sensor_hx711_weight
 * @name Weight
 * The weight variable from an Avia Semiconductor HX711
 *
 * {{ @ref AviaSemiHX711_Weight::AviaSemiHX711_Weight }}
 */
/**@{*/
/// @brief Sensor variable number; weight is stored in sensorValues[0].
#define HX711_WEIGHT_VAR_NUM 0
/// @brief Decimals places in string representation; weight should have 3 -
/// resolution is 0.001 kg.
#define HX711_WEIGHT_RESOLUTION 4
/// @brief Variable name in
/// [ODM2 controlled vocabulary](http://vocabulary.odm2.org/variablename/);
/// "temperature"
#define HX711_WEIGHT_VAR_NAME "weight"
/// @brief Variable unit name in
/// [ODM2 controlled vocabulary](http://vocabulary.odm2.org/units/);
/// "Kilogram" (°C)
#define HX711_WEIGHT_UNIT_NAME "kilogram"
/// @brief Default variable short code; "AviaSemiHX711Weight"
#define HX711_WEIGHT_DEFAULT_CODE "AviaSemiHX711Weight"
/**@}*/

/**
 * @anchor sensor_hx711_raw
 * @name Raw
 * The raw reading variable from an Avia Semiconductor HX711
 *
 * {{ @ref AviaSemiHX711_Raw::AviaSemiHX711_Raw }}
 */
/**@{*/
/// @brief Sensor variable number; raw value is stored in sensorValues[1].
#define HX711_RAW_VAR_NUM 1
/// @brief Decimals places in string representation; weight should have 3 -
/// resolution is 0.001 kg.
#define HX711_RAW_RESOLUTION 0
/// @brief Variable name in
/// [ODM2 controlled vocabulary](http://vocabulary.odm2.org/variablename/);
/// "relativeHumidity"
#define HX711_RAW_VAR_NAME "raw"
/// @brief Variable unit name in
/// [ODM2 controlled vocabulary](http://vocabulary.odm2.org/units/);
#define HX711_RAW_UNIT_NAME ""
/// @brief Default variable short code; "AviaSemiHX711Raw"
#define HX711_RAW_DEFAULT_CODE "AviaSemiHX711Raw"
/**@}*/


/* clang-format off */
/**
 * @brief The Sensor sub-class for the [HX711](@ref sensor_hx711).
 *
 * @ingroup sensor_hx711
 */
/* clang-format on */
class AviaSemiHX711 : public Sensor {
public:
    /**
     * @brief Construct a new HX711 object.
     *
     * @param doutPin The data out pin
     * @param sckPin The clock pin
     * @param loadCellOffset Offset value for the load scale
     * @param loadCellDivider Divider value for the load scale
     */
    explicit AviaSemiHX711(uint8_t doutPin, uint8_t sckPin,
                           long loadCellOffset, long loadCellDivider);
    /**
     * @brief Destroy the HX711 object
     */
    ~AviaSemiHX711();

    /**
     * @brief Wake the sensor up, if necessary.  Do whatever it takes to get a
     * sensor in the proper state to begin a measurement.
     *
     * Verifies that the power is on and updates the #_sensorStatus.  This also
     * sets the #_millisSensorActivated timestamp.
     *
     * @note This does NOT include any wait for sensor readiness.
     *
     * @return **bool** True if the wake function completed successfully.
     */
    //bool wake(void) override;
    //bool sleep(void) override;
    void powerUp(void) override;
    void powerDown(void) override;
    /**
     * @brief Do any one-time preparations needed before the sensor will be able
     * to take readings.
     *
     * @return **bool** True if the setup was successful.
     */
    bool setup(void) override;
    /**
     * @copydoc Sensor::getSensorLocation()
     */
    String getSensorLocation(void) override;

    // bool startSingleMeasurement(void) override;  // for forced mode
    /**
     * @copydoc Sensor::addSingleMeasurementResult()
     */
    bool addSingleMeasurementResult(void) override;

private:
    /**
     * @brief Internal reference the the HX711 lowlevel library object
     */
    HX711 hx711_internal;
    /**
     * @brief The pin parameters for the HX711 and parameters for the load scale.
     */
    uint8_t _doutPin;
    uint8_t _sckPin;
    long _loadCellOffset;
    long _loadCellDivider;
};


/* clang-format off */
/**
 * @brief The Variable sub-class used for the
 * [temperature output](@ref sensor_hx711_weight)
 * from a [HX711](@ref sensor_hx711).
 *
 * @ingroup sensor_hx711
 */
/* clang-format on */
class AviaSemiHX711_Weight : public Variable {
public:
    /**
     * @brief Construct a new AviaSemiHX711_Weight object.
     *
     * @param parentSense The parent AviaSemiHX711 providing the result
     * values.
     * @param uuid A universally unique identifier (UUID or GUID) for the
     * variable; optional with the default value of an empty string.
     * @param varCode A short code to help identify the variable in files;
     * optional with a default value of "AviaSemiHX711Weight".
     */
    explicit AviaSemiHX711_Weight(AviaSemiHX711* parentSense, const char* uuid = "",
                              const char* varCode = HX711_WEIGHT_DEFAULT_CODE)
            : Variable(parentSense, (const uint8_t)HX711_WEIGHT_VAR_NUM,
                       (uint8_t)HX711_WEIGHT_RESOLUTION, HX711_WEIGHT_VAR_NAME,
                       HX711_WEIGHT_UNIT_NAME, varCode, uuid) {}
    /**
     * @brief Construct a new AviaSemiHX711_Weight object.
     *
     * @note This must be tied with a parent AviaSemiHX711 before it can be used.
     */
    AviaSemiHX711_Weight()
            : Variable((const uint8_t)HX711_WEIGHT_VAR_NUM,
                       (uint8_t)HX711_WEIGHT_RESOLUTION, HX711_WEIGHT_VAR_NAME,
                       HX711_WEIGHT_UNIT_NAME, HX711_WEIGHT_DEFAULT_CODE) {}
    /**
     * @brief Destroy the AviaSemiHX711_Weight object - no action needed.
     */
    ~AviaSemiHX711_Weight() {}
};


/* clang-format off */
/**
 * @brief The Variable sub-class used for the
 * [relative humidity output](@ref sensor_hx711_humidity) from a
 * [HX711](@ref sensor_hx711).
 *
 * @ingroup sensor_hx711
 */
/* clang-format on */
class AviaSemiHX711_Raw : public Variable {
public:
    /**
     * @brief Construct a new AviaSemiHX711_Raw object.
     *
     * @param parentSense The parent AviaSemiHX711 providing the result
     * values.
     * @param uuid A universally unique identifier (UUID or GUID) for the
     * variable; optional with the default value of an empty string.
     * @param varCode A short code to help identify the variable in files;
     * optional with a default value of "AviaSemiHX711Raw".
     */
    explicit AviaSemiHX711_Raw(
            AviaSemiHX711* parentSense, const char* uuid = "",
            const char* varCode = HX711_RAW_DEFAULT_CODE)
            : Variable(parentSense, (const uint8_t)HX711_RAW_VAR_NUM,
                       (uint8_t)HX711_RAW_RESOLUTION,
                       HX711_RAW_VAR_NAME, HX711_RAW_UNIT_NAME, varCode,
                       uuid) {}
    /**
     * @brief Construct a new AviaSemiHX711_Raw object.
     *
     * @note This must be tied with a parent AviaSemiHX711 before it can be used.
     */
    AviaSemiHX711_Raw()
            : Variable((const uint8_t)HX711_RAW_VAR_NUM,
                       (uint8_t)HX711_RAW_RESOLUTION,
                       HX711_RAW_VAR_NAME, HX711_RAW_UNIT_NAME,
                       HX711_RAW_DEFAULT_CODE) {}
    /**
     * @brief Destroy the AviaSemiHX711_Raw object - no action needed.
     */
    ~AviaSemiHX711_Raw() {}
};
/**@}*/
#endif  // SRC_SENSORS_AVIASEMIHX711_H_
