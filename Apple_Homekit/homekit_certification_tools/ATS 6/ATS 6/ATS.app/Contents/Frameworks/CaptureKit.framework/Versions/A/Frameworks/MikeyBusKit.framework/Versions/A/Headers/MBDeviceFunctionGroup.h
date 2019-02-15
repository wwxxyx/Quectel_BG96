//
//  MBDeviceFunctionGroup.h
//  MikeyBusKit
//
//  Created by Vinod Madigeri on 10/9/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MikeyBusKit/MBTypes.h>

@class MBDeviceFunctionGroupProperty;
@class MBConfigurationDesignatorPacket;

@interface MBDeviceFunctionGroup : NSObject

/**
 Function Group Number is slightly different from Function Group ID. They are the same except for VendorSpecific function group where Group number is 0x01 and Group ID is 0xFF.
 */
@property (nonatomic) NSNumber *functionGroupNumber;

/**
 Function group type for thie function group.
 */
@property (nonatomic) MBFunctionGroupType functionGroupType;

/**
 Returns the calculated Group ID based on Group number and Group type.
 */
@property (nonatomic, readonly) MBFunctionGroupID functionGroupID;

/**
 Property ID to property info map.
 */
@property (nonatomic, strong) NSMutableDictionary <NSNumber *, MBDeviceFunctionGroupProperty *> *functionGroupPropertyInfos;

/**
 Method to read the function group properties from MBConfigurationCommandRetFunctionGroupProperties response.
 
 @param retFunctionGroupProperties Configuration designator packet with function group properties.
 */
- (void)processRetFunctionGroupProperties:(MBConfigurationDesignatorPacket *)retFunctionGroupProperties;

@end
