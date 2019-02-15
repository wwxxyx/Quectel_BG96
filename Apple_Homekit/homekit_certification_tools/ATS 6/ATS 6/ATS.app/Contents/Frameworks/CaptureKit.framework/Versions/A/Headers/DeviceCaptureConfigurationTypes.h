//
//  DeviceCaptureConfigurationTypes.h
//  ATS
//
//  Created by Natália Cabral on 9/24/14.
//  Copyright 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
// VM: We should be re-using CaptureConfiguration
typedef NS_ENUM(UInt8, DeviceTransportType) {
    DeviceTransportTypeSerial = 0x00,
    DeviceTransportTypeUSBHID = 0x01,
    DeviceTransportTypeBluetooth = 0x02,
    DeviceTransportTypeUSBBulkPipe = 0x03,
    DeviceTransportTypeWiFi = 0x04,
    DeviceTransportTypeMikeyBus = 0x05,
    DeviceTransportTypeUnknown = 0xFF,
};

#define DeviceTransportTypeUART DeviceTransportTypeSerial
#define DeviceTransportTypeAppleDeviceUSBDevice DeviceTransportTypeUSBHID
#define DeviceTransportTypeAppleDeviceUSBHost DeviceTransportTypeUSBBulkPipe

typedef NS_ENUM(UInt8, DeviceProtocolType) {
    DeviceProtocolTypeiAP1 = 0x00,
    DeviceProtocolTypeiAP2 = 0x01,
    DeviceProtocolTypeUnknown = 0xFF,
};

typedef NS_ENUM(UInt8, DeviceConnectorType) {
    DeviceConnectorTypeLightning = 0x00,
    DeviceConnectorType30pin = 0x01,
    DeviceConnectorTypeBluetooth = 0x02,
    DeviceConnectorTypeWiFi = 0x03,
    DeviceConnectorTypeSmartConnector = 0x04,
    DeviceConnectorTypeLightningAudioConnector = 0x05,
    DeviceConnectorTypeUnknown = 0xFF,
};

typedef NS_ENUM(UInt8, AccessoryType) {
    /// Any feature other than the rest defined in this enum (ex: CarPlay and AirPlay 2)
    AccessoryTypeOther = 0x00,
    /// Feature type CarPlay
    AccessoryTypeCarPlay = 0x01,
    /// Feature type AirPlay 2
    AccessoryTypeAirPlay2 = 0x02,
};
