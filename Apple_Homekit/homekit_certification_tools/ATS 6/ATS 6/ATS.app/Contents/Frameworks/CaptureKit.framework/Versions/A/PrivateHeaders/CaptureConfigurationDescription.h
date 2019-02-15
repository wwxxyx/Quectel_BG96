//
//  CaptureConfigurationDescription.h
//  ATSMacApp
//
//  Created by Sergio Sette on 10/28/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/DeviceCaptureConfigurationTypes.h>

NSString *const transportTypeNameFromDeviceTransportType(DeviceTransportType deviceTransportType);
NSString *const protocolTypeNameFromDeviceProtocolType(DeviceProtocolType deviceProtocolType);
NSString *const connectorTypeNameFromDeviceConnectorType(DeviceConnectorType deviceConnectorType);
NSString *const accessoryTypeNameFromAccessoryType(AccessoryType accessoryType);

@interface CaptureConfigurationDescription : NSObject

@property (nonatomic) DeviceTransportType transportType;
@property (nonatomic) DeviceConnectorType connectorType;
@property (nonatomic) DeviceProtocolType protocolType;
@property (nonatomic) AccessoryType accessoryType;
@property (nonatomic) BOOL isEmulated;

+ (CaptureConfigurationDescription *)configurationWithTransport:(DeviceTransportType)transportType
                                                  connectorType:(DeviceConnectorType)connectorType
                                                   protocolType:(DeviceProtocolType)protocolType
                                                    accessoryType:(AccessoryType)accessoryType
                                                     isEmulated:(BOOL)isEmulated;

@end
