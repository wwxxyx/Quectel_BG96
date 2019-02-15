//
//  MBRetFunctionGroupPropertyInfoConfigurationDesignator.h
//  MikeyBusKit
//
//  Created by Vinod Madigeri on 11/5/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <MikeyBusKit/MBConfigurationDesignatorPacket.h>

/*
 String name for Property Info Data.
 */
extern NSString * const MBRetFunctionGroupPropertyInfoDataKey;

@interface MBRetFunctionGroupPropertyInfoConfigurationDesignator : MBConfigurationDesignatorPacket

/**
 Parsed property info data.
 */
@property (nonatomic, readonly) NSData *propertyInfoData;

@end
