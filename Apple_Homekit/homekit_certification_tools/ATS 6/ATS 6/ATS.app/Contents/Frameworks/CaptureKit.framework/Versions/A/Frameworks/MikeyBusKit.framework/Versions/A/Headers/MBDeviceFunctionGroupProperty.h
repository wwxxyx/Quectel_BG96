//
//  MBDeviceFunctionGroupProperty.h
//  MikeyBusKit
//
//  Created by Vinod Madigeri on 10/9/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MBConfigurationDesignatorPacket;

@interface MBDeviceFunctionGroupProperty : NSObject

/**
 Property ID for this function group property.
 */
@property (nonatomic, strong) NSNumber *propertyID;

/**
 Property information in Function Group-specific format.
 */
@property (nonatomic, strong) NSData *propertyInformationData;

/**
 Method to read the function group property info from MBConfigurationCommandRetFunctionGroupPropertyInfo response.
 
 @param retFunctionGroupPropertyInfo Configuration designator packet with function group property info.
 */
- (void)processRetFunctionGroupPropertyInfo:(MBConfigurationDesignatorPacket *)retFunctionGroupPropertyInfo;

/**
 Method to read the function group property info from MBConfigurationCommandSetFunctionGroupPropertyInfo response.
 
 @param setFunctionGroupPropertyInfo Configuration designator packet with function group property info.
 */
- (void)processSetFunctionGroupPropertyInfo:(MBConfigurationDesignatorPacket *)setFunctionGroupPropertyInfo;

@end
