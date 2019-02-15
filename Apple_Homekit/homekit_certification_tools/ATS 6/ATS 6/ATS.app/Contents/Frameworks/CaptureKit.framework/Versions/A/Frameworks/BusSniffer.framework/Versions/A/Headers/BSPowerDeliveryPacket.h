//
//  BSPowerDeliveryPacket.h
//  BusSniffer
//
//  Created by Zachary Church on 3/29/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BSPowerDeliveryTypes.h"
#import "BusSniffer/BSPowerDeliveryDataObject.h"

@interface BSPowerDeliveryPacket : NSObject

@property (readonly, strong) NSData *data;
@property (readonly, strong) NSString *commandName;
@property (readonly, strong) NSString *commandNameWithID;
@property (readonly) BSPDCommandType commandType;
@property (readonly) BSPDDataCommand dataCommand;
@property (readonly) BSPDControlCommand controlCommand;
@property (readonly, strong) NSArray <BSPowerDeliveryDataObject *> *dataObjects;
@property (readonly, strong, nonatomic) NSError *error;

+ (instancetype)packetWithData:(NSData *)data forCommand:(UInt8)command capabilities:(BSPowerDeliveryPacket *)capabilities;

@end
