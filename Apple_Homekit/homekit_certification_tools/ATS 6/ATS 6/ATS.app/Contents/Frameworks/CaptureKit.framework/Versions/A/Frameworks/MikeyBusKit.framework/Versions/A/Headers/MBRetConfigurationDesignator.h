//
//  MBRetConfigurationDesignator.h
//  MikeyBusKit
//
//  Created by Vinod Madigeri on 11/5/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <MikeyBusKit/MBConfigurationDesignatorPacket.h>

/*
 String Keys to be able to access each value inside parsedPayloadFields
 */
extern NSString * const MBConfigurationClassKey;
extern NSString * const MBConfigurationSubclassKey;
extern NSString * const MBNumberOfFunctionGroupsKey;
extern NSString * const MBFunctionGroupNumberKey;


@interface MBRetConfigurationDesignator : MBConfigurationDesignatorPacket

/**
 Dictionary of Function Group Numbers and their respective group types map parsed from Ret Configuration Designator payload.
 */
@property (nonatomic, readonly) NSDictionary <NSNumber *, NSNumber *> *functionGroupNumberToGroupTypeMap;

@end
