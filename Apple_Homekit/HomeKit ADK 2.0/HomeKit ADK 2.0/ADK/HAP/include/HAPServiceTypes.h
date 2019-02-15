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

#ifndef HAP_SERVICE_TYPES_H
#define HAP_SERVICE_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "HAP.h"

#if __has_feature(nullability)
#pragma clang assume_nonnull begin
#endif

/**
 * Accessory Information.
 *
 * Every accessory must expose a single instance of the Accessory information service with the following definition. The
 * values of Manufacturer, Model, Name and Serial Number must be persistent through the lifetime of the accessory.
 *
 * Any other Apple-defined characteristics added to this service must only contain one or more of the following
 * permissions: Paired Read or Notify. Custom characteristics added to this service must only contain one or more of the
 * following permissions: Paired Read, Notify, or Broadcast. All other permissions are not permitted.
 *
 * Required Characteristics:
 * - Identify
 * - Manufacturer
 * - Model
 * - Name
 * - Serial Number
 * - Firmware Revision
 *
 * Optional Characteristics:
 * - Hardware Revision
 * - Accessory Flags
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.1 Accessory Information
 */
/**@{*/
#define kHAPServiceDebugDescription_AccessoryInformation \
    "accessory-information"

extern const HAPUUID kHAPServiceType_AccessoryInformation;
/**@}*/

/**
 * Fan.
 *
 * This service describes a fan.
 *
 * Required Characteristics:
 * - On
 *
 * Optional Characteristics:
 * - Rotation Direction
 * - Rotation Speed
 * - Name
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.3 Fan
 */
/**@{*/
#define kHAPServiceDebugDescription_Fan \
    "fan"

extern const HAPUUID kHAPServiceType_Fan;
/**@}*/

/**
 * Garage Door Opener.
 *
 * This service describes a garage door opener that controls a single door. If a garage has more than one door, then
 * each door should have its own Garage Door Opener Service.
 *
 * Required Characteristics:
 * - Current Door State
 * - Target Door State
 * - Obstruction Detected
 *
 * Optional Characteristics:
 * - Lock Current State
 * - Lock Target State
 * - Name
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.4 Garage Door Opener
 */
/**@{*/
#define kHAPServiceDebugDescription_GarageDoorOpener \
    "garage-door-opener"

extern const HAPUUID kHAPServiceType_GarageDoorOpener;
/**@}*/

/**
 * Lightbulb.
 *
 * This service describes a lightbulb.
 *
 * Required Characteristics:
 * - On
 *
 * Optional Characteristics:
 * - Brightness
 * - Hue
 * - Name
 * - Saturation
 * - Color Temperature
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.5 Lightbulb
 */
/**@{*/
#define kHAPServiceDebugDescription_Lightbulb \
    "lightbulb"

extern const HAPUUID kHAPServiceType_Lightbulb;
/**@}*/

/**
 * Lock Management.
 *
 * The HomeKit Lock Management Service is designed to expose deeper interaction with a Lock device.
 *
 * Required Characteristics:
 * - Lock Control Point
 * - Version
 *
 * Optional Characteristics:
 * - Logs
 * - Audio Feedback
 * - Lock Management Auto Security Timeout
 * - Administrator Only Access
 * - Lock Last Known Action
 * - Current Door State
 * - Motion Detected
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.6 Lock Management
 */
/**@{*/
#define kHAPServiceDebugDescription_LockManagement \
    "lock-management"

extern const HAPUUID kHAPServiceType_LockManagement;
/**@}*/

/**
 * Lock Mechanism.
 *
 * The HomeKit Lock Mechanism Service is designed to expose and control the physical lock mechanism on a device.
 *
 * Required Characteristics:
 * - Lock Current State
 * - Lock Target State
 *
 * Optional Characteristics:
 * - Name
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.7 Lock Mechanism
 */
/**@{*/
#define kHAPServiceDebugDescription_LockMechanism \
    "lock-mechanism"

extern const HAPUUID kHAPServiceType_LockMechanism;
/**@}*/

/**
 * Outlet.
 *
 * This service describes a power outlet.
 *
 * Required Characteristics:
 * - On
 * - Outlet In Use
 *
 * Optional Characteristics:
 * - Name
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.8 Outlet
 */
/**@{*/
#define kHAPServiceDebugDescription_Outlet \
    "outlet"

extern const HAPUUID kHAPServiceType_Outlet;
/**@}*/

/**
 * Switch.
 *
 * This service describes a binary switch.
 *
 * Required Characteristics:
 * - On
 *
 * Optional Characteristics:
 * - Name
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.9 Switch
 */
/**@{*/
#define kHAPServiceDebugDescription_Switch \
    "switch"

extern const HAPUUID kHAPServiceType_Switch;
/**@}*/

/**
 * Thermostat.
 *
 * This service describes a thermostat.
 *
 * Required Characteristics:
 * - Current Heating Cooling State
 * - Target Heating Cooling State
 * - Current Temperature
 * - Target Temperature
 * - Temperature Display Units
 *
 * Optional Characteristics:
 * - Cooling Threshold Temperature
 * - Current Relative Humidity
 * - Heating Threshold Temperature
 * - Name
 * - Target Relative Humidity
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.10 Thermostat
 */
/**@{*/
#define kHAPServiceDebugDescription_Thermostat \
    "thermostat"

extern const HAPUUID kHAPServiceType_Thermostat;
/**@}*/

/**
 * Pairing.
 *
 * Defines characteristics to support pairing between a controller and an accessory.
 *
 * Required Characteristics:
 * - Pair Setup
 * - Pair Verify
 * - Pairing Features
 * - Pairing Pairings
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 5.13.1 Pairing Service
 */
/**@{*/
#define kHAPServiceDebugDescription_Pairing \
    "pairing"

extern const HAPUUID kHAPServiceType_Pairing;
/**@}*/

/**
 * Security System.
 *
 * This service describes a security system service.
 *
 * This service requires iOS 9 or later.
 *
 * Required Characteristics:
 * - Security System Current State
 * - Security System Target State
 *
 * Optional Characteristics:
 * - Name
 * - Security System Alarm Type
 * - Status Fault
 * - Status Tampered
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.12 Security System
 */
/**@{*/
#define kHAPServiceDebugDescription_SecuritySystem \
    "security-system"

extern const HAPUUID kHAPServiceType_SecuritySystem;
/**@}*/

/**
 * Carbon Monoxide Sensor.
 *
 * This service describes a Carbon Monoxide Sensor.
 *
 * This service requires iOS 9 or later.
 *
 * Required Characteristics:
 * - Carbon Monoxide Detected
 *
 * Optional Characteristics:
 * - Name
 * - Status Active
 * - Status Fault
 * - Status Tampered
 * - Status Low Battery
 * - Carbon Monoxide Level
 * - Carbon Monoxide Peak Level
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.13 Carbon Monoxide Sensor
 */
/**@{*/
#define kHAPServiceDebugDescription_CarbonMonoxideSensor \
    "sensor.carbon-monoxide"

extern const HAPUUID kHAPServiceType_CarbonMonoxideSensor;
/**@}*/

/**
 * Contact Sensor.
 *
 * This service describes a Contact Sensor.
 *
 * This service requires iOS 9 or later.
 *
 * Required Characteristics:
 * - Contact Sensor State
 *
 * Optional Characteristics:
 * - Name
 * - Status Active
 * - Status Fault
 * - Status Tampered
 * - Status Low Battery
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.14 Contact Sensor
 */
/**@{*/
#define kHAPServiceDebugDescription_ContactSensor \
    "sensor.contact"

extern const HAPUUID kHAPServiceType_ContactSensor;
/**@}*/

/**
 * Door.
 *
 * This service describes a motorized door.
 *
 * This service requires iOS 9 or later.
 *
 * Required Characteristics:
 * - Current Position
 * - Target Position
 * - Position State
 *
 * Optional Characteristics:
 * - Name
 * - Hold Position
 * - Obstruction Detected
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.15 Door
 */
/**@{*/
#define kHAPServiceDebugDescription_Door \
    "door"

extern const HAPUUID kHAPServiceType_Door;
/**@}*/

/**
 * Humidity Sensor.
 *
 * This service describes a Humidity Sensor.
 *
 * This service requires iOS 9 or later.
 *
 * Required Characteristics:
 * - Current Relative Humidity
 *
 * Optional Characteristics:
 * - Name
 * - Status Active
 * - Status Fault
 * - Status Tampered
 * - Status Low Battery
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.16 Humidity Sensor
 */
/**@{*/
#define kHAPServiceDebugDescription_HumiditySensor \
    "sensor.humidity"

extern const HAPUUID kHAPServiceType_HumiditySensor;
/**@}*/

/**
 * Leak Sensor.
 *
 * This service describes a Leak Sensor.
 *
 * This service requires iOS 9 or later.
 *
 * Required Characteristics:
 * - Leak Detected
 *
 * Optional Characteristics:
 * - Name
 * - Status Active
 * - Status Fault
 * - Status Tampered
 * - Status Low Battery
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.17 Leak Sensor
 */
/**@{*/
#define kHAPServiceDebugDescription_LeakSensor \
    "sensor.leak"

extern const HAPUUID kHAPServiceType_LeakSensor;
/**@}*/

/**
 * Light Sensor.
 *
 * This service describes a Light Sensor.
 *
 * This service requires iOS 9 or later.
 *
 * Required Characteristics:
 * - Current Ambient Light Level
 *
 * Optional Characteristics:
 * - Name
 * - Status Active
 * - Status Fault
 * - Status Tampered
 * - Status Low Battery
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.18 Light Sensor
 */
/**@{*/
#define kHAPServiceDebugDescription_LightSensor \
    "sensor.light"

extern const HAPUUID kHAPServiceType_LightSensor;
/**@}*/

/**
 * Motion Sensor.
 *
 * This service describes a Motion Sensor.
 *
 * This service requires iOS 9 or later.
 *
 * Required Characteristics:
 * - Motion Detected
 *
 * Optional Characteristics:
 * - Name
 * - Status Active
 * - Status Fault
 * - Status Tampered
 * - Status Low Battery
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.19 Motion Sensor
 */
/**@{*/
#define kHAPServiceDebugDescription_MotionSensor \
    "sensor.motion"

extern const HAPUUID kHAPServiceType_MotionSensor;
/**@}*/

/**
 * Occupancy Sensor.
 *
 * This service describes an Occupancy Sensor.
 *
 * This service requires iOS 9 or later.
 *
 * Required Characteristics:
 * - Occupancy Detected
 *
 * Optional Characteristics:
 * - Name
 * - Status Active
 * - Status Fault
 * - Status Tampered
 * - Status Low Battery
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.20 Occupancy Sensor
 */
/**@{*/
#define kHAPServiceDebugDescription_OccupancySensor \
    "sensor.occupancy"

extern const HAPUUID kHAPServiceType_OccupancySensor;
/**@}*/

/**
 * Smoke Sensor.
 *
 * This service describes a Smoke detector Sensor.
 *
 * This service requires iOS 9 or later.
 *
 * Required Characteristics:
 * - Smoke Detected
 *
 * Optional Characteristics:
 * - Name
 * - Status Active
 * - Status Fault
 * - Status Tampered
 * - Status Low Battery
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.21 Smoke Sensor
 */
/**@{*/
#define kHAPServiceDebugDescription_SmokeSensor \
    "sensor.smoke"

extern const HAPUUID kHAPServiceType_SmokeSensor;
/**@}*/

/**
 * Stateless Programmable Switch.
 *
 * This service describes a stateless programmable switch.
 *
 * The following rules apply to a stateless programmable switch accessory:
 * - Each physical switch on the accessory must be represented by a unique instance of this service.
 * - If there are multiple instances of this service on the accessory, they must be linked to a Service Label.
 * - If there are multiple instances of this service on the accessory, Service Label Index is a required characteristic.
 * - Service Label Index value for each instance of this service linked to the same Service Label must be unique.
 * - The User visible label on the physical accessory should match the Service Label Namespace described by the
 *   accessory.
 * - If there is only one instance of this service on the accessory, Service Label is not required and consequently
 *   Service Label Index must not be present.
 *
 * This service requires iOS 10.3 or later.
 *
 * Required Characteristics:
 * - Programmable Switch Event
 *
 * Optional Characteristics:
 * - Name
 * - Service Label Index
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.22 Stateless Programmable Switch
 */
/**@{*/
#define kHAPServiceDebugDescription_StatelessProgrammableSwitch \
    "stateless-programmable-switch"

extern const HAPUUID kHAPServiceType_StatelessProgrammableSwitch;
/**@}*/

/**
 * Temperature Sensor.
 *
 * This service describes a Temperature Sensor.
 *
 * This service requires iOS 9 or later.
 *
 * Required Characteristics:
 * - Current Temperature
 *
 * Optional Characteristics:
 * - Name
 * - Status Active
 * - Status Fault
 * - Status Low Battery
 * - Status Tampered
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.23 Temperature Sensor
 */
/**@{*/
#define kHAPServiceDebugDescription_TemperatureSensor \
    "sensor.temperature"

extern const HAPUUID kHAPServiceType_TemperatureSensor;
/**@}*/

/**
 * Window.
 *
 * This service describes a motorized window.
 *
 * This service requires iOS 9 or later.
 *
 * Required Characteristics:
 * - Current Position
 * - Target Position
 * - Position State
 *
 * Optional Characteristics:
 * - Name
 * - Hold Position
 * - Obstruction Detected
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.24 Window
 */
/**@{*/
#define kHAPServiceDebugDescription_Window \
    "window"

extern const HAPUUID kHAPServiceType_Window;
/**@}*/

/**
 * Window Covering.
 *
 * This service describes motorized window coverings or shades - examples include shutters, blinds, awnings etc.
 *
 * This service requires iOS 9 or later.
 *
 * Required Characteristics:
 * - Target Position
 * - Current Position
 * - Position State
 *
 * Optional Characteristics:
 * - Name
 * - Hold Position
 * - Current Horizontal Tilt Angle
 * - Target Horizontal Tilt Angle
 * - Current Vertical Tilt Angle
 * - Target Vertical Tilt Angle
 * - Obstruction Detected
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.25 Window Covering
 */
/**@{*/
#define kHAPServiceDebugDescription_WindowCovering \
    "window-covering"

extern const HAPUUID kHAPServiceType_WindowCovering;
/**@}*/

/**
 * Air Quality Sensor.
 *
 * This service describes an air quality sensor. Air Quality refers to the cumulative air quality recorded by the
 * accessory which may be based on multiple sensors present.
 *
 * This service requires iOS 9 or later and is updated in iOS 10.
 *
 * Required Characteristics:
 * - Air Quality
 *
 * Optional Characteristics:
 * - Name
 * - Ozone Density
 * - Nitrogen Dioxide Density
 * - Sulphur Dioxide Density
 * - PM2.5 Density
 * - PM10 Density
 * - VOC Density
 * - Status Active
 * - Status Fault
 * - Status Tampered
 * - Status Low Battery
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.11 Air Quality Sensor
 */
/**@{*/
#define kHAPServiceDebugDescription_AirQualitySensor \
    "sensor.air-quality"

extern const HAPUUID kHAPServiceType_AirQualitySensor;
/**@}*/

/**
 * Battery Service.
 *
 * This service describes a battery service.
 *
 * This service requires iOS 9 or later.
 *
 * Required Characteristics:
 * - Battery Level
 * - Charging State
 * - Status Low Battery
 *
 * Optional Characteristics:
 * - Name
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.26 Battery Service
 */
/**@{*/
#define kHAPServiceDebugDescription_BatteryService \
    "battery"

extern const HAPUUID kHAPServiceType_BatteryService;
/**@}*/

/**
 * Carbon Dioxide Sensor.
 *
 * This service describes a Carbon Dioxide Sensor.
 *
 * This service requires iOS 9 or later.
 *
 * Required Characteristics:
 * - Carbon Dioxide Detected
 *
 * Optional Characteristics:
 * - Name
 * - Status Active
 * - Status Fault
 * - Status Tampered
 * - Status Low Battery
 * - Carbon Dioxide Level
 * - Carbon Dioxide Peak Level
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.27 Carbon Dioxide Sensor
 */
/**@{*/
#define kHAPServiceDebugDescription_CarbonDioxideSensor \
    "sensor.carbon-dioxide"

extern const HAPUUID kHAPServiceType_CarbonDioxideSensor;
/**@}*/

/**
 * HAP Protocol Information.
 *
 * Every accessory must expose a single instance of the HAP protocol information. For a bridge accessory, only the
 * primary HAP accessory object must contain this service. The Version value is transport dependent. Refer to BLE
 * Protocol Version Characteristic for BLE protocol version. Refer to IP Protocol Version for IP protocol version.
 *
 * Required Characteristics:
 * - Version
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.2 HAP Protocol Information
 */
/**@{*/
#define kHAPServiceDebugDescription_HAPProtocolInformation \
    "protocol.information.service"

extern const HAPUUID kHAPServiceType_HAPProtocolInformation;
/**@}*/

/**
 * Fan v2.
 *
 * This service describes a fan.
 *
 * If the fan service is included in air purifier accessories, Current Fan State and Target Fan State are required
 * characteristics.
 *
 * This service requires iOS 10.3 or later.
 *
 * Required Characteristics:
 * - Active
 *
 * Optional Characteristics:
 * - Name
 * - Current Fan State
 * - Target Fan State
 * - Rotation Direction
 * - Rotation Speed
 * - Swing Mode
 * - Lock Physical Controls
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.32 Fan v2
 */
/**@{*/
#define kHAPServiceDebugDescription_FanV2 \
    "fanv2"

extern const HAPUUID kHAPServiceType_FanV2;

/**
 * Slat.
 *
 * This service describes a slat which tilts on a vertical or a horizontal axis.
 *
 * Current Tilt Angle and Target Tilt Angle may be included in this service if the user can set the slats to a
 * particular tilt angle.
 *
 * Swing Mode implies that the slats can swing automatically (e.g. vents on a fan).
 *
 * This service requires iOS 10.3 or later.
 *
 * Required Characteristics:
 * - Current Slat State
 * - Slat Type
 *
 * Optional Characteristics:
 * - Name
 * - Swing Mode
 * - Current Tilt Angle
 * - Target Tilt Angle
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.33 Slat
 */
/**@{*/
#define kHAPServiceDebugDescription_Slat \
    "vertical-slat"

extern const HAPUUID kHAPServiceType_Slat;
/**@}*/

/**
 * Filter Maintenance.
 *
 * This service can be used to describe maintenance operations for a filter.
 *
 * This service requires iOS 10.3 or later.
 *
 * Required Characteristics:
 * - Filter Change Indication
 *
 * Optional Characteristics:
 * - Name
 * - Filter Life Level
 * - Reset Filter Indication
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.34 Filter Maintenance
 */
/**@{*/
#define kHAPServiceDebugDescription_FilterMaintenance \
    "filter-maintenance"

extern const HAPUUID kHAPServiceType_FilterMaintenance;
/**@}*/

/**
 * Air Purifier.
 *
 * This service describes an air purifier. An air purifier accessory can have additional linked services such as:
 * - Filter Maintenance service(s) to describe one or more air filters.
 * - Air Quality Sensor services to describe air quality sensors.
 * - Fan v2 service to describe a fan which can be independently controlled.
 * - Slat service to control vents.
 *
 * If Fan v2 is included as a linked service in an air purifier accessory:
 * - Changing Active characteristic on the Air Purifier must result in corresponding change to Active characteristic on
 *   the Fan v2.
 * - Changing Active characteristic on the Fan v2 from Inactive to Active does not require the Active on the Air
 *   Purifier to change. This enables Fan Only mode on air purifier.
 * - Changing Active characteristic on the Fan v2 from Active to Inactive must result in the Active on the Air Purifier
 *   to change to Inactive.
 *
 * An air purifier accessory service may include Rotation Speed to control fan speed if the fan cannot be independently
 * controlled.
 *
 * This service requires iOS 10.3 or later.
 *
 * Required Characteristics:
 * - Active
 * - Current Air Purifier State
 * - Target Air Purifier State
 *
 * Optional Characteristics:
 * - Name
 * - Rotation Speed
 * - Swing Mode
 * - Lock Physical Controls
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.35 Air Purifier
 */
/**@{*/
#define kHAPServiceDebugDescription_AirPurifier \
    "air-purifier"

extern const HAPUUID kHAPServiceType_AirPurifier;
/**@}*/

/**
 * Heater Cooler.
 *
 * This service can be used to describe either of the following:
 * - a heater
 * - a cooler
 * - a heater and a cooler
 *
 * A heater/cooler accessory may have additional:
 * - Fan v2 service to describe a fan which can be independently controlled
 * - Slat service to control vents
 *
 * A heater must include Heating Threshold Temperature. A cooler must include Cooling Threshold Temperature.
 *
 * A heater/cooler accessory service may include Rotation Speed to control fan speed if the fan cannot be independently
 * controlled.
 *
 * This service requires iOS 10.3 or later.
 *
 * Required Characteristics:
 * - Active
 * - Current Temperature
 * - Current Heater Cooler State
 * - Target Heater Cooler State
 *
 * Optional Characteristics:
 * - Name
 * - Rotation Speed
 * - Temperature Display Units
 * - Swing Mode
 * - Cooling Threshold Temperature
 * - Heating Threshold Temperature
 * - Lock Physical Controls
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.36 Heater Cooler
 */
/**@{*/
#define kHAPServiceDebugDescription_HeaterCooler \
    "heater-cooler"

extern const HAPUUID kHAPServiceType_HeaterCooler;
/**@}*/

/**
 * Humidifier Dehumidifier.
 *
 * This service can be used to describe either of the following:
 * - an air humidifier
 * - an air dehumidifier
 * - an air humidifier and an air dehumidifier
 *
 * An air humidifier/dehumidifier accessory may have additional:
 * - Fan v2 service to describe a fan which can be independently controlled
 * - Slat service to control vents
 *
 * A dehumidifier must include Relative Humidity Dehumidifier Threshold. A humidifier must include Relative Humidity
 * Humidifier Threshold.
 *
 * A humidifier/dehumidifier accessory service may include Rotation Speed to control fan speed if the fan cannot be
 * independently controlled.
 *
 * This service requires iOS 10.3 or later.
 *
 * Required Characteristics:
 * - Active
 * - Current Relative Humidity
 * - Current Humidifier Dehumidifier State
 * - Target Humidifier Dehumidifier State
 *
 * Optional Characteristics:
 * - Name
 * - Relative Humidity Dehumidifier Threshold
 * - Relative Humidity Humidifier Threshold
 * - Rotation Speed
 * - Swing Mode
 * - Water Level
 * - Lock Physical Controls
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.37 Humidifier Dehumidifier
 */
/**@{*/
#define kHAPServiceDebugDescription_HumidifierDehumidifier \
    "humidifier-dehumidifier"

extern const HAPUUID kHAPServiceType_HumidifierDehumidifier;
/**@}*/

/**
 * Service Label.
 *
 * This service describes label scheme.
 *
 * This service requires iOS 10.3 or later.
 *
 * Required Characteristics:
 * - Service Label Namespace
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.38 Service Label
 */
/**@{*/
#define kHAPServiceDebugDescription_ServiceLabel \
    "service-label"

extern const HAPUUID kHAPServiceType_ServiceLabel;
/**@}*/

/**
 * Irrigation System.
 *
 * This service describes an irrigation system. This service must be present on an irrigation systems which supports
 * on-device schedules or supports a top-level `Active` control across multiple valves.
 *
 * A sprinkler system accessory maybe:
 *
 * - a combination of `Irrigation System` service on a bridge accessory with a collection of one or more `Valve`
 *   services (with `Valve Type` set to `Irrigation`) as bridged accessories (The bridge accessory is typically
 *   connected to each valve using wires). OR
 *
 * - a combination of `Irrigation System` service with a collection of one or more linked `Valve` services (with
 *   `Valve Type` set to `Irrigation`) (The valves are collocated e.g. hose based system). OR
 *
 * - a combination of `Valve` service(s) with `Valve Type` set to `Irrigation` (e.g., a system with one or more valves
 *   which does not support scheduling)
 *
 * An irrigation system is set to `Active` when the system is enabled. When one of the valves is set to `In Use`, the
 * irrigation system must be set to in use.
 *
 * An accessory which includes this services must include the `Set Duration` in the `Valve`.
 *
 * An irrigation system accessory which does not auto detect the number of valves it is connected to and requires user
 * to provide this information must include the `Is Configured` in the `Valve`.
 *
 * `Remaining Duration` on this service implies the total remaining duration to water across all valves.
 *
 * This service requires iOS 11.2 or later.
 *
 * Required Characteristics:
 * - Active
 * - Program Mode
 * - In Use
 *
 * Optional Characteristics:
 * - Remaining Duration
 * - Name
 * - Status Fault
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.39 Irrigation System
 */
/**@{*/
#define kHAPServiceDebugDescription_IrrigationSystem \
    "irrigation-system"

extern const HAPUUID kHAPServiceType_IrrigationSystem;
/**@}*/

/**
 * Valve.
 *
 * This service describes accessories like irrigation valves or water outlets. A valve is set to `In Use` when there are
 * fluid flowing through the valve.
 *
 * If an accessory has this service with `Valve Type` set to Irrigation it must include the `Set Duration` and
 * `Remaining Duration` characteristic on the `Valve` service.
 *
 * `Service Label Index` must be present on each instance of this service if the accessory consists of:
 *
 * - a bridge accessory (the `Service Label` service must be included here) which includes multiple bridged accessories
 *   each with `Valve` service.
 *
 * - an accessory (the `Service Label` service must be included here) which includes multiple linked `Valve` services.
 *
 * If an accessory has this service with `Service Label Index` included, the default `Name` must be empty string unless
 * user has already assigned a name to this valve before accessory is HomeKit paired. In such a case, the default name
 * should be the user configured name for this valve.
 *
 * `Is Configured` must be present on each instance of this service if the accessory is used in an irrigation system or
 * shower system where all valves may not be configured to use (e.g., depending on physical wire connection).
 *
 * Setting the value of `Active` to `Active` on this service must result in `Irrigation System` bridge to be set to
 * `Active` if this service is used in context of an Irrigation system.
 *
 * This service requires iOS 11.2 or later.
 *
 * Required Characteristics:
 * - Active
 * - In Use
 * - Valve Type
 *
 * Optional Characteristics:
 * - Set Duration
 * - Remaining Duration
 * - Is Configured
 * - Service Label Index
 * - Status Fault
 * - Name
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.40 Valve
 */
/**@{*/
#define kHAPServiceDebugDescription_Valve \
    "valve"

extern const HAPUUID kHAPServiceType_Valve;
/**@}*/

/**
 * Faucet.
 *
 * This service describes accessories like faucets or shower heads. This service must only be included when an accessory
 * has either a linked `Heater Cooler` with single linked `Valve` service or multiple linked `Valve` services
 * (with/without `Heater Cooler` service) to describe water outlets. This service serves as a top level service for such
 * accessories.
 *
 * A faucet which supports heating of water must include `Heater Cooler` and `Valve` service as linked services. An
 * accessory which supports one or multiple water outlets and heating of water through a common temperature control,
 * must include `Heater Cooler` and `Valve` service(s) as linked services to the faucet service.
 *
 * Setting the value of `Active` to `InActive` on this service must turn off the faucet accessory. The accessory must
 * retain the state of `Active` characteristics on any linked `Valve` services when the `Active` on this service is
 * toggled. The accessory must set the value of `Active` to `InActive` of any linked `Heater Cooler` service when the
 * `Active` on this service is set to `InActive`.
 *
 * This service requires iOS 11.2 or later.
 *
 * Required Characteristics:
 * - Active
 *
 * Optional Characteristics:
 * - Status Fault
 * - Name
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.41 Faucet
 */
/**@{*/
#define kHAPServiceDebugDescription_Faucet \
    "faucet"

extern const HAPUUID kHAPServiceType_Faucet;
/**@}*/

/**
 * Camera RTP Stream Management.
 *
 * A Camera RTP Stream Management service allows description of the supported audio and video codecs and parameters
 * supported by the accessory as well as configuration and control of the RTP session to stream the audio/video stream
 * to a controller.
 *
 * This service requires iOS 10 or later.
 *
 * Required Characteristics:
 * - Streaming Status
 * - Supported Video Stream Configuration
 * - Supported Audio Stream Configuration
 * - Supported RTP Configuration
 * - Setup Endpoints
 * - Selected RTP Stream Configuration
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.28 Camera RTP Stream Management
 */
/**@{*/
#define kHAPServiceDebugDescription_CameraRTPStreamManagement \
    "camera-rtp-stream-management"

extern const HAPUUID kHAPServiceType_CameraRTPStreamManagement;
/**@}*/

/**
 * Microphone.
 *
 * A Microphone service is used to control the sourcing of the input audio - primarily through a microphone.
 *
 * This service requires iOS 10 or later.
 *
 * Required Characteristics:
 * - Mute
 *
 * Optional Characteristics:
 * - Name
 * - Volume
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.29 Microphone
 */
/**@{*/
#define kHAPServiceDebugDescription_Microphone \
    "microphone"

extern const HAPUUID kHAPServiceType_Microphone;
/**@}*/

/**
 * Speaker.
 *
 * A Speaker service is to use to control the audio output settings on a speaker device.
 *
 * This service requires iOS 10 or later.
 *
 * Required Characteristics:
 * - Mute
 *
 * Optional Characteristics:
 * - Name
 * - Volume
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.30 Speaker
 */
/**@{*/
#define kHAPServiceDebugDescription_Speaker \
    "speaker"

extern const HAPUUID kHAPServiceType_Speaker;
/**@}*/

/**
 * Doorbell.
 *
 * The Doorbell Service describes a doorbell and is the primary service of the Video Doorbell Profile.
 *
 * This service requires iOS 10 or later.
 *
 * Required Characteristics:
 * - Programmable Switch Event
 *
 * Optional Characteristics:
 * - Name
 * - Volume
 * - Brightness
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.31 Doorbell
 */
/**@{*/
#define kHAPServiceDebugDescription_Doorbell \
    "doorbell"

extern const HAPUUID kHAPServiceType_Doorbell;
/**@}*/

/**
 * Target Control Management.
 *
 * This service manages the configuration for a remote and allows the accessory to indicate the supported configuration.
 *
 * This service requires iOS 12 or later.
 *
 * Required Characteristics:
 * - Target Control Supported Configuration
 * - Target Control List
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.42 Target Control Management
 */
/**@{*/
#define kHAPServiceDebugDescription_TargetControlManagement \
    "target-control-management"

extern const HAPUUID kHAPServiceType_TargetControlManagement;
/**@}*/

/**
 * Target Control.
 *
 * This service handles the control of a selected target from the remote. If an accessory can support control of
 * multiple concurrent Apple TVs at the same time without requiring the user to select an Apple TV on the remote UI, it
 * must expose multiple instances of this service.
 *
 * This service requires iOS 12 or later.
 *
 * Required Characteristics:
 * - Active Identifier
 * - Active
 * - Button Event
 *
 * Optional Characteristics:
 * - Name
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.43 Target Control
 */
/**@{*/
#define kHAPServiceDebugDescription_TargetControl \
    "target-control"

extern const HAPUUID kHAPServiceType_TargetControl;
/**@}*/

/**
 * Audio Stream Management.
 *
 * This service manages the configuration for audio input from the remote, if applicable.
 *
 * This service requires iOS 12 or later.
 *
 * Required Characteristics:
 * - Supported Audio Stream Configuration
 * - Selected Audio Stream Configuration
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.44 Audio Stream Management
 */
/**@{*/
#define kHAPServiceDebugDescription_AudioStreamManagement \
    "audio-stream-management"

extern const HAPUUID kHAPServiceType_AudioStreamManagement;
/**@}*/

/**
 * Siri.
 *
 * This service allows configuration and management of Siri.
 *
 * This service must be linked to the Audio Stream Management and Data Stream Transport Management.
 *
 * This service requires iOS 12 or later.
 *
 * Required Characteristics:
 * - Siri Input Type
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.46 Siri
 */
/**@{*/
#define kHAPServiceDebugDescription_Siri \
    "siri"

extern const HAPUUID kHAPServiceType_Siri;
/**@}*/

/**
 * Data Stream Transport Management.
 *
 * This service allows configuration and management of the transport data stream.
 *
 * This service requires iOS 12.
 *
 * Required Characteristics:
 * - Supported Data Stream Transport Configuration
 * - Setup Data Stream Transport
 * - Version
 *
 * @see HomeKit Accessory Protocol Specification - Developer Preview 1 for R13
 *      Section 9.45 Data Stream Transport Management
 */
/**@{*/
#define kHAPServiceDebugDescription_DataStreamTransportManagement \
    "data-stream-transport-management"
    
extern const HAPUUID kHAPServiceType_DataStreamTransportManagement;
/**@}*/

#if __has_feature(nullability)
#pragma clang assume_nonnull end
#endif

#ifdef __cplusplus
}
#endif

#endif
