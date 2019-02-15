//
//  MBSetFunctionGroupPropertyInfoConfigurationDesignator.h
//  MikeyBusKit
//
//  Created by Vinod Madigeri on 11/5/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <MikeyBusKit/MBConfigurationDesignatorPacket.h>

/*
 String Keys to be able to access each value inside parsedPayloadFields
 */
extern NSString * const MBSetFunctionGroupPropertyInfoFunctionGroupNumberKey;
extern NSString * const MBSetFunctionGroupPropertyInfoPropertyIDKey;

/**
 String name for property info data.
 */
extern NSString * const MBSetFunctionGroupPropertyInfoDataKey;

@interface MBSetFunctionGroupPropertyInfoConfigurationDesignator : MBConfigurationDesignatorPacket

/**
 Parsed property info data.
 */
@property (nonatomic, readonly) NSData *propertyInfoData;

@end
