//
//  MBRetFunctionGroupPropertiesConfigurationDesignator.h
//  MikeyBusKit
//
//  Created by Vinod Madigeri on 11/5/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <MikeyBusKit/MBConfigurationDesignatorPacket.h>

/*
 String name for Property IDs.
 */
extern NSString * const MBRetFunctionGroupPropertiesPropertyIDKey;

@interface MBRetFunctionGroupPropertiesConfigurationDesignator : MBConfigurationDesignatorPacket

/**
 Parsed property ID's for RetFunctionGroupProperties.
 */
@property (nonatomic, readonly) NSArray <NSNumber *> *propertyIDs;

@end
