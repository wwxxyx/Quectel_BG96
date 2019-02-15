//
// Package: HomeKit-ADK
// Version: 2.0 (16A62)
//
// Disclaimer: IMPORTANT: This Apple software is supplied to you, by Apple Inc. ("Apple"), in your
// capacity as a current, and in good standing, Licensee in the MFi Licensing Program. Use of this
// Apple software is governed by and subject to the terms and conditions of your MFi License,
// including, but not limited to, the restrictions specified in the provision entitled "Public
// Software", and is further subject to your agreement to the following additional terms, and your
// agreement that the use, installation, modification or redistribution of this Apple software
// constitutes acceptance of these additional terms. If you do not agree with these additional terms,
// you may not use, install, modify or redistribute this Apple software.
//
// Subject to all of these terms and in consideration of your agreement to abide by them, Apple grants
// you, for as long as you are a current and in good-standing MFi Licensee, a personal, non-exclusive
// license, under Apple's copyrights in this Apple software (the "Apple Software"), to use,
// reproduce, and modify the Apple Software in source form, and to use, reproduce, modify, and
// redistribute the Apple Software, with or without modifications, in binary form, in each of the
// foregoing cases to the extent necessary to develop and/or manufacture "Proposed Products" and
// "Licensed Products" in accordance with the terms of your MFi License. While you may not
// redistribute the Apple Software in source form, should you redistribute the Apple Software in binary
// form, you must retain this notice and the following text and disclaimers in all such redistributions
// of the Apple Software. Neither the name, trademarks, service marks, or logos of Apple Inc. may be
// used to endorse or promote products derived from the Apple Software without specific prior written
// permission from Apple. Except as expressly stated in this notice, no other rights or licenses,
// express or implied, are granted by Apple herein, including but not limited to any patent rights that
// may be infringed by your derivative works or by other works in which the Apple Software may be
// incorporated. Apple may terminate this license to the Apple Software by removing it from the list
// of Licensed Technology in the MFi License, or otherwise in accordance with the terms of such MFi License.
//
// Unless you explicitly state otherwise, if you provide any ideas, suggestions, recommendations, bug
// fixes or enhancements to Apple in connection with this software ("Feedback"), you hereby grant to
// Apple a non-exclusive, fully paid-up, perpetual, irrevocable, worldwide license to make, use,
// reproduce, incorporate, modify, display, perform, sell, make or have made derivative works of,
// distribute (directly or indirectly) and sublicense, such Feedback in connection with Apple products
// and services. Providing this Feedback is voluntary, but if you do provide Feedback to Apple, you
// acknowledge and agree that Apple may exercise the license granted above without the payment of
// royalties or further consideration to Participant.
//
// The Apple Software is provided by Apple on an "AS IS" basis. APPLE MAKES NO WARRANTIES, EXPRESS OR
// IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR
// IN COMBINATION WITH YOUR PRODUCTS.
//
// IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION
// AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
// (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (C) 2018 Apple Inc. All Rights Reserved.
//

// An example that implements the thermostat homekit profile. It can serve as a basic implementation for
// any platform. The accessory logic implementation is reduced to internal state updates and log output.
//
// This implementation is platform-independent.
//
// The code consists of multiple parts:
//
//   1. The definition of the accessory configuration and its internal state.
//
//   2. Helper functions to load and save the state of the accessory.
//
//   3. The definitions for the HomeKit attribute database.
//
//   4. Helper functions that modify the state of the current heating and cooling state
//      based on the accessory state.
//
//   5. The callbacks that implement the actual behavior of the accessory, in this
//      case here they merely access the global accessory state variable and write
//      to the log to make the behavior easily observable.
//
//   6. The initialization of the accessory state.
//
//   7. Callbacks that notify the server in case their associated value has changed.

#include "HAP.h"

#include "App.h"
#include "DB.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Domain used in the key value store for application data.
 *
 * Purged: On factory reset.
 */
#define kAppKeyValueStoreDomain_Configuration                                   ((HAPPlatformKeyValueStoreDomain) 0x00)

/**
 * Key used in the key value store to store the configuration state.
 *
 * Purged: On factory reset.
 */
#define kAppKeyValueStoreKey_Configuration_State                                ((HAPPlatformKeyValueStoreDomain) 0x00)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Global accessory configuration.
 */
typedef struct {
    struct {
        struct {
            /**
             * This value describes the current mode of a device that supports cooling or heating its environment
             */
            HAPCharacteristicValue_CurrentHeatingCoolingState current;
            /**
             * This value describes the target mode of a device that supports heating/cooling, e.g. a thermostat.
             */
            HAPCharacteristicValue_TargetHeatingCoolingState target;
        } heatingCoolingState;

        struct {
            /**
             * This value describes the current temperature of the environment in Celsius irrespective of display units
             * chosen.
             */
            float current;
            /**
             * This characteristic describes the target temperature in Celsius that the device is actively attempting to
             * reach.
             */
            float target;
            /**
             * This characteristic describes units of temperature used for presentation purposes.
             */
            HAPCharacteristicValue_TemperatureDisplayUnits displayUnits;
            /**
             * This value represents the ʼmaximum temperatureʼ that must be reached before cooling is turned on.
             */
            float coolingThreshold;
            /**
             * This value represents the ʼminimum temperatureʼ that must be reached before heating is turned on.
             */
            float heatingThreshold;
        } temperature;

        struct {
            /**
             * This value describes the current relative humidity of the environment that contains the device. The value
             * is expressed in percentage (%).
             */
            float current;
            /**
             * This value describes the target relative humidity that the device is actively attempting to reach. The
             * value is expressed in percentage (%).
             */
            float target;
        } relativeHumidity;
    } state;
    HAPAccessoryServerRef *server;
    HAPPlatformKeyValueStoreRef keyValueStore;
} AccessoryConfiguration;

static AccessoryConfiguration accessoryConfiguration;

//----------------------------------------------------------------------------------------------------------------------

/**
 * Initialize a default accessory state that validates with the device configuration.
 */
static void SetupDefaultAccessoryState(void)
{
    accessoryConfiguration.state.heatingCoolingState.current =
        kHAPCharacteristicValue_CurrentHeatingCoolingState_Off;
    accessoryConfiguration.state.heatingCoolingState.target =
        kHAPCharacteristicValue_TargetHeatingCoolingState_Off;

    accessoryConfiguration.state.temperature.current =
        thermostatTemperatureCurrent.constraints.minimumValue;
    accessoryConfiguration.state.temperature.target =
        thermostatTemperatureTarget.constraints.minimumValue;
    accessoryConfiguration.state.temperature.displayUnits =
        kHAPCharacteristicValue_TemperatureDisplayUnits_Celsius;
    accessoryConfiguration.state.temperature.coolingThreshold =
        thermostatTemperatureCoolingThreshold.constraints.maximumValue;
    accessoryConfiguration.state.temperature.heatingThreshold =
        thermostatTemperatureHeatingThreshold.constraints.minimumValue;

    accessoryConfiguration.state.relativeHumidity.current =
        thermostatRelativeHumidityCurrent.constraints.minimumValue;
    accessoryConfiguration.state.relativeHumidity.target =
        thermostatRelativeHumidityTarget.constraints.minimumValue;
}

/**
 * Load the accessory state from persistent memory.
 */
static void LoadAccessoryState(void)
{
    HAPPrecondition(accessoryConfiguration.keyValueStore);
    
    HAPError err;
    
    // Load persistent state if available
    bool found;
    size_t numBytes;
    
    err = HAPPlatformKeyValueStoreGet(accessoryConfiguration.keyValueStore,
        kAppKeyValueStoreDomain_Configuration, kAppKeyValueStoreKey_Configuration_State,
        &accessoryConfiguration.state, sizeof accessoryConfiguration.state, &numBytes, &found);
    
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        HAPFatalError();
    }
    if (!found || numBytes != sizeof accessoryConfiguration.state) {
        if (found) {
            HAPLogError(&kHAPLog_Default,
                "Unexpected app state found in key-value store. Resetting to default.");
        }
        SetupDefaultAccessoryState();
    }
}

/**
 * Save the accessory state to persistent memory
 */
static void SaveAccessoryState(void)
{
    HAPPrecondition(accessoryConfiguration.keyValueStore);

    HAPError err;
    err = HAPPlatformKeyValueStoreSet(accessoryConfiguration.keyValueStore,
        kAppKeyValueStoreDomain_Configuration, kAppKeyValueStoreKey_Configuration_State,
        &accessoryConfiguration.state, sizeof accessoryConfiguration.state);
    if (err) {
        HAPAssert(err == kHAPError_Unknown);
        HAPFatalError();
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * HomeKit accessory that provides the Thermostat service.
 */
static const HAPAccessory accessory = {
    .aid = 1,
    .category = kHAPAccessoryCategory_Thermostats,
    .name = "Acme Thermostat",
    .manufacturer = "Acme",
    .model = "Thermostat,1",
    .serialNumber = "099DB48E9E29",
    .firmwareVersion = "1",
    .hardwareVersion = "1",
    .services = (const HAPService *const[]) {
        &accessoryInformationService,
        &hapProtocolInformationService,
        &pairingService,
        &thermostatService,
        NULL
    },
    .cameraStreamConfigurations = NULL,
    .callbacks = {
        .identify = IdentifyAccessory
    }
};

//----------------------------------------------------------------------------------------------------------------------

/**
 * Helper function: returns the absolute value of a float
 */
static float Abs(
    float value)
{
    if (value < 0.0f) {
        return -1.0f * value;
    }
    return value;
}

/**
 * Returns the string associated to 'Current Heating Cooling State'.
 */
static const char *GetCurrentHeatingCoolingStateDescription(
    HAPCharacteristicValue_CurrentHeatingCoolingState state)
{
    switch (state) {
        case kHAPCharacteristicValue_CurrentHeatingCoolingState_Off: {
            return "Off.";
        }
        case kHAPCharacteristicValue_CurrentHeatingCoolingState_Heat: {
            return "Heat. The Heater is currently on.";
        }
        case kHAPCharacteristicValue_CurrentHeatingCoolingState_Cool: {
            return "Cool. Cooler is currently on.";
        }
    }
    HAPFatalError();
}

/**
 * Returns the string associated to 'Target Heating Cooling State'.
 */
static const char *GetTargetHeatingCoolingStateDescription(
    HAPCharacteristicValue_TargetHeatingCoolingState state)
{
    switch (state) {
        case kHAPCharacteristicValue_TargetHeatingCoolingState_Off: {
            return "Off";
        }
        case kHAPCharacteristicValue_TargetHeatingCoolingState_Heat: {
            return "Heat. If the current temperature is below the target temperature then turn on heating.";
        }
        case kHAPCharacteristicValue_TargetHeatingCoolingState_Cool: {
            return "Cool. If the current temperature is above the target temperature then turn on cooling.";
        }
        case kHAPCharacteristicValue_TargetHeatingCoolingState_Auto: {
            return "Auto. Turn on heating or cooling to maintain temperature within"
                " the heating and cooling threshold of the target temperature.";
        }
    }
    HAPFatalError();
}

/**
 * Returns the string associated to 'Temperature Display Units'.
 */
static const char *GetTemperatureDisplayUnitsDescription(
    HAPCharacteristicValue_TemperatureDisplayUnits units)
{
    switch (units) {
        case kHAPCharacteristicValue_TemperatureDisplayUnits_Celsius: {
            return "Celsius";
        }
        case kHAPCharacteristicValue_TemperatureDisplayUnits_Fahrenheit: {
            return "Fahrenheit";
        }
    }
    HAPFatalError();
}

/**
 * Change the 'Current Heating Cooling State` and notify if the value has changed.
 */
static void ThermostatChangeHeatingCoolingStateCurrent(
    HAPAccessoryServerRef *server,
    HAPCharacteristicValue_CurrentHeatingCoolingState newValue)
{
    HAPPrecondition(server);
    
    if (newValue != accessoryConfiguration.state.heatingCoolingState.current) {
        HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, GetCurrentHeatingCoolingStateDescription(newValue));
        
        accessoryConfiguration.state.heatingCoolingState.current = newValue;
        HAPAccessoryServerRaiseEvent(
            server,
            &thermostatHeatingCoolingCurrent,
            &thermostatService,
            &accessory);
    }
}

/**
 * Disable the thermostat
 */
static void ThermostatOff(
    HAPAccessoryServerRef *server)
{
    ThermostatChangeHeatingCoolingStateCurrent(server, kHAPCharacteristicValue_CurrentHeatingCoolingState_Off);
}

/**
 * Heat if the current temperature is below the target temperature
 */
static void ThermostatHeat(
    HAPAccessoryServerRef *server)
{
    if (accessoryConfiguration.state.temperature.current < accessoryConfiguration.state.temperature.target) {
        ThermostatChangeHeatingCoolingStateCurrent(server, kHAPCharacteristicValue_CurrentHeatingCoolingState_Heat);
    } else {
        ThermostatOff(server);
    }
}

/**
 * Cool if the current temperature is above the target temperature
 */
static void ThermostatCool(
    HAPAccessoryServerRef *server)
{
    if (accessoryConfiguration.state.temperature.current > accessoryConfiguration.state.temperature.target) {
        ThermostatChangeHeatingCoolingStateCurrent(server, kHAPCharacteristicValue_CurrentHeatingCoolingState_Cool);
    } else {
        ThermostatOff(server);
    }
}

/**
 * Auto mode for the thermostat
 * - Heat if the current temperature is below the heating threshold
 * - Cool if the current temperature is above the heating threshold
 */
static void ThermostatAuto(
    HAPAccessoryServerRef *server)
{
    float currentTemperature = accessoryConfiguration.state.temperature.current;
    
    if (currentTemperature < accessoryConfiguration.state.temperature.heatingThreshold) {
        ThermostatChangeHeatingCoolingStateCurrent(server, kHAPCharacteristicValue_CurrentHeatingCoolingState_Heat);
    } else if (currentTemperature > accessoryConfiguration.state.temperature.coolingThreshold) {
        ThermostatChangeHeatingCoolingStateCurrent(server, kHAPCharacteristicValue_CurrentHeatingCoolingState_Cool);
    } else {
        ThermostatOff(server);
    }
}

/**
 * Update the accessory state
 */
static void UpdateAccessoryState(
    HAPAccessoryServerRef *server)
{
    switch (accessoryConfiguration.state.heatingCoolingState.target) {
        case kHAPCharacteristicValue_TargetHeatingCoolingState_Off: {
            ThermostatOff(server);
        } break;
        case kHAPCharacteristicValue_TargetHeatingCoolingState_Heat: {
            ThermostatHeat(server);
        } break;
        case kHAPCharacteristicValue_TargetHeatingCoolingState_Cool: {
            ThermostatCool(server);
        } break;
        case kHAPCharacteristicValue_TargetHeatingCoolingState_Auto: {
            ThermostatAuto(server);
        } break;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**
 * Identify routine. Used to locate the accessory.
 */
HAP_RESULT_USE_CHECK
HAPError IdentifyAccessory(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPAccessoryIdentifyRequest *request HAP_UNUSED,
    void *_Nullable context HAP_UNUSED)
{
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    return kHAPError_None;
}

/**
 * Handle read request on the 'Current Temperature' characteristic of the Thermostat service
 */
HAP_RESULT_USE_CHECK
HAPError HandleThermostatCurrentTemperatureRead(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPFloatCharacteristicReadRequest *request HAP_UNUSED,
    float *value,
    void *_Nullable context HAP_UNUSED)
{
    *value = accessoryConfiguration.state.temperature.current;
    HAPLogInfo(&kHAPLog_Default, "%s: %g", __func__, (double) *value);
    return kHAPError_None;
}

/**
 * Handle read request on the 'Target Temperature' characteristic of the Thermostat service
 */
HAP_RESULT_USE_CHECK
HAPError HandleThermostatTargetTemperatureRead(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPFloatCharacteristicReadRequest *request HAP_UNUSED,
    float *value,
    void *_Nullable context HAP_UNUSED)
{
    *value = accessoryConfiguration.state.temperature.target;
    HAPLogInfo(&kHAPLog_Default, "%s: %g", __func__, (double) *value);
    return kHAPError_None;
}

/**
 * Handle write request on the 'Target Temperature' characteristic of the Thermostat service
 */
HAP_RESULT_USE_CHECK
HAPError HandleThermostatTargetTemperatureWrite(
    HAPAccessoryServerRef *server,
    const HAPFloatCharacteristicWriteRequest *request,
    float value,
    void *_Nullable context HAP_UNUSED)
{
    HAPLogInfo(&kHAPLog_Default, "%s: %g", __func__, (double) value);
    if (Abs(accessoryConfiguration.state.temperature.target - value) > FLT_MIN) {
        accessoryConfiguration.state.temperature.target = value;
        UpdateAccessoryState(server);
        SaveAccessoryState();
        HAPAccessoryServerRaiseEvent(
            server,
            request->characteristic,
            request->service,
            request->accessory);
    }
    return kHAPError_None;
}

/**
 * Handle read request on the 'Temperature Display Units' characteristic of the Thermostat service
 */
HAP_RESULT_USE_CHECK
HAPError HandleThermostatTemperatureDisplayUnitsRead(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPUInt8CharacteristicReadRequest *request HAP_UNUSED,
    uint8_t *value,
    void *_Nullable context HAP_UNUSED)
{
    *value = (HAPCharacteristicValue_TemperatureDisplayUnits) accessoryConfiguration.state.temperature.displayUnits;
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__,
        GetTemperatureDisplayUnitsDescription((HAPCharacteristicValue_TemperatureDisplayUnits) *value));
    return kHAPError_None;
}

/**
 * Handle write request on the 'Temperature Display Units' characteristic of the Thermostat service
 */
HAP_RESULT_USE_CHECK
HAPError HandleThermostatTemperatureDisplayUnitsWrite(
    HAPAccessoryServerRef *server,
    const HAPUInt8CharacteristicWriteRequest *request,
    uint8_t value,
    void *_Nullable context HAP_UNUSED)
{
    HAPCharacteristicValue_TemperatureDisplayUnits displayUnits =
        (HAPCharacteristicValue_TemperatureDisplayUnits) value;
    
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, GetTemperatureDisplayUnitsDescription(displayUnits));
    if (accessoryConfiguration.state.temperature.displayUnits != displayUnits) {
        accessoryConfiguration.state.temperature.displayUnits = displayUnits;
        UpdateAccessoryState(server);
        SaveAccessoryState();
        HAPAccessoryServerRaiseEvent(
            server,
            request->characteristic,
            request->service,
            request->accessory);
    }
    return kHAPError_None;
}

/**
 * Handle read request on the 'Current Heating Cooling State' characteristic of the Thermostat service
 */
HAP_RESULT_USE_CHECK
HAPError HandleThermostatCurrentHeatingCoolingStateRead(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPUInt8CharacteristicReadRequest *request HAP_UNUSED,
    uint8_t *value,
    void *_Nullable context HAP_UNUSED)
{
    *value = (uint8_t) accessoryConfiguration.state.heatingCoolingState.current;
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__,
        GetCurrentHeatingCoolingStateDescription((HAPCharacteristicValue_CurrentHeatingCoolingState) *value));
    return kHAPError_None;
}

/**
 * Handle read request on the 'Target Heating Cooling State' characteristic of the Thermostat service
 */
HAP_RESULT_USE_CHECK
HAPError HandleThermostatTargetHeatingCoolingStateRead(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPUInt8CharacteristicReadRequest *request HAP_UNUSED,
    uint8_t *value,
    void *_Nullable context HAP_UNUSED)
{
    *value = (uint8_t) accessoryConfiguration.state.heatingCoolingState.target;
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__,
        GetTargetHeatingCoolingStateDescription((HAPCharacteristicValue_TargetHeatingCoolingState) *value));
    return kHAPError_None;
}

/**
 * Handle write request on the 'Target Heating Cooling State"' characteristic of the Thermostat service
 */
HAP_RESULT_USE_CHECK
HAPError HandleThermostatTargetHeatingCoolingStateWrite(
    HAPAccessoryServerRef *server,
    const HAPUInt8CharacteristicWriteRequest *request,
    uint8_t value,
    void *_Nullable context HAP_UNUSED)
{
    HAPCharacteristicValue_TargetHeatingCoolingState state =
        (HAPCharacteristicValue_TargetHeatingCoolingState) value;
    
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, GetTargetHeatingCoolingStateDescription(state));
    if (accessoryConfiguration.state.heatingCoolingState.target != state) {
        accessoryConfiguration.state.heatingCoolingState.target = state;
        UpdateAccessoryState(server);
        SaveAccessoryState();
        HAPAccessoryServerRaiseEvent(
            server,
            request->characteristic,
            request->service,
            request->accessory);
    }
    return kHAPError_None;
}

/**
 * Handle read request on the 'Cooling Threshold Temperature' characteristic of the Thermostat service
 */
HAP_RESULT_USE_CHECK
HAPError HandleThermostatCoolingThresholdTemperatureRead(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPFloatCharacteristicReadRequest *request HAP_UNUSED,
    float *value,
    void *_Nullable context HAP_UNUSED)
{
    *value = accessoryConfiguration.state.temperature.coolingThreshold;
    HAPLogInfo(&kHAPLog_Default, "%s: %g", __func__, (double) *value);
    return kHAPError_None;
}

/**
 * Handle write request on the 'Cooling Threshold Temperature' characteristic of the Thermostat service
 */
HAP_RESULT_USE_CHECK
HAPError HandleThermostatCoolingThresholdTemperatureWrite(
    HAPAccessoryServerRef *server,
    const HAPFloatCharacteristicWriteRequest *request,
    float value,
    void *_Nullable context HAP_UNUSED)
{
    HAPLogInfo(&kHAPLog_Default, "%s: %g", __func__, (double) value);
    if (Abs(accessoryConfiguration.state.temperature.coolingThreshold - value) > FLT_MIN) {
        accessoryConfiguration.state.temperature.coolingThreshold = value;
        UpdateAccessoryState(server);
        SaveAccessoryState();
        HAPAccessoryServerRaiseEvent(
            server,
            request->characteristic,
            request->service,
            request->accessory);
    }
    return kHAPError_None;
}

/**
 * Handle read request on the 'Heating Threshold Temperature' characteristic of the Thermostat service
 */
HAP_RESULT_USE_CHECK
HAPError HandleThermostatHeatingThresholdTemperatureRead(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPFloatCharacteristicReadRequest *request HAP_UNUSED,
    float *value,
    void *_Nullable context HAP_UNUSED)
{
    *value = accessoryConfiguration.state.temperature.heatingThreshold;
    HAPLogInfo(&kHAPLog_Default, "%s: %g", __func__, (double) *value);
    return kHAPError_None;
}

/**
 * Handle write request on the 'Heating Threshold Temperature' characteristic of the Thermostat service
 */
HAP_RESULT_USE_CHECK
HAPError HandleThermostatHeatingThresholdTemperatureWrite(
    HAPAccessoryServerRef *server,
    const HAPFloatCharacteristicWriteRequest *request,
    float value,
    void *_Nullable context HAP_UNUSED)
{
    HAPLogInfo(&kHAPLog_Default, "%s: %g", __func__, (double) value);
    if (Abs(accessoryConfiguration.state.temperature.heatingThreshold - value) > FLT_MIN) {
        accessoryConfiguration.state.temperature.heatingThreshold = value;
        UpdateAccessoryState(server);
        SaveAccessoryState();
        HAPAccessoryServerRaiseEvent(
            server,
            request->characteristic,
            request->service,
            request->accessory);
    }
    return kHAPError_None;
}

/**
 * Handle read request on the 'Current Relative Humidity' characteristic of the Thermostat service
 */
HAP_RESULT_USE_CHECK
HAPError HandleThermostatCurrentRelativeHumidityRead(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPFloatCharacteristicReadRequest *request HAP_UNUSED,
    float *value,
    void *_Nullable context HAP_UNUSED)
{
    *value = accessoryConfiguration.state.relativeHumidity.current;
    HAPLogInfo(&kHAPLog_Default, "%s: %g", __func__, (double) *value);
    return kHAPError_None;
}

/**
 * Handle read request on the 'Target Relative Humidity' characteristic of the Thermostat service
 */
HAP_RESULT_USE_CHECK
HAPError HandleThermostatTargetRelativeHumidityRead(
    HAPAccessoryServerRef *server HAP_UNUSED,
    const HAPFloatCharacteristicReadRequest *request HAP_UNUSED,
    float *value,
    void *_Nullable context HAP_UNUSED)
{
    *value = accessoryConfiguration.state.relativeHumidity.target;
    HAPLogInfo(&kHAPLog_Default, "%s: %g", __func__, (double) *value);
    return kHAPError_None;
}

/**
 * Handle write request on the 'Target Relative Humidity' characteristic of the Thermostat service
 */
HAP_RESULT_USE_CHECK
HAPError HandleThermostatTargetRelativeHumidityWrite(
    HAPAccessoryServerRef *server,
    const HAPFloatCharacteristicWriteRequest *request,
    float value,
    void *_Nullable context HAP_UNUSED)
{
    HAPLogInfo(&kHAPLog_Default, "%s: %g", __func__, (double) value);
    if (Abs(accessoryConfiguration.state.relativeHumidity.target - value) > FLT_MIN) {
        accessoryConfiguration.state.relativeHumidity.target = value;
        UpdateAccessoryState(server);
        SaveAccessoryState();
        HAPAccessoryServerRaiseEvent(
            server,
            request->characteristic,
            request->service,
            request->accessory);
    }
    return kHAPError_None;
}

//----------------------------------------------------------------------------------------------------------------------

void AppCreate(
    HAPAccessoryServerRef *server,
    HAPPlatformKeyValueStoreRef keyValueStore)
{
    HAPPrecondition(server);
    HAPPrecondition(keyValueStore);
    
    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    
    HAPRawBufferZero(&accessoryConfiguration, sizeof accessoryConfiguration);
    accessoryConfiguration.server = server;
    accessoryConfiguration.keyValueStore = keyValueStore;
    LoadAccessoryState();
}

void AppRelease(void)
{
}

void AppAccessoryServerStart(void)
{
    HAPAccessoryServerStart(accessoryConfiguration.server, &accessory);
}

//----------------------------------------------------------------------------------------------------------------------

void AccessoryServerHandleUpdatedState(
    HAPAccessoryServerRef *server,
    void *_Nullable context)
{
    HAPPrecondition(server);
    HAPPrecondition(!context);
    
    switch (HAPAccessoryServerGetState(server)) {
        case kHAPAccessoryServerState_Idle: {
            HAPLogInfo(&kHAPLog_Default, "Accessory Server State did update: Idle.");
        } return;
        case kHAPAccessoryServerState_Running: {
            HAPLogInfo(&kHAPLog_Default, "Accessory Server State did update: Running.");
        } return;
        case kHAPAccessoryServerState_Stopping: {
            HAPLogInfo(&kHAPLog_Default, "Accessory Server State did update: Stopping.");
        } return;
    }
    HAPFatalError();
}
