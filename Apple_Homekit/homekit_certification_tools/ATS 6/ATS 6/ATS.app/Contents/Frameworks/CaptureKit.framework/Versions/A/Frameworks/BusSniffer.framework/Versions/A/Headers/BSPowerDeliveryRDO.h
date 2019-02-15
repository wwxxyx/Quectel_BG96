//
//  BSPowerDeliveryRDO.h
//  BusSniffer
//
//  Created by Zachary Church on 3/30/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BSPowerDeliveryDataObject.h"

@class BSPowerDeliveryPacket;
@class BSPowerDeliveryPDO;

@interface BSPowerDeliveryRDO : BSPowerDeliveryDataObject

@property (readonly, nonatomic, strong) BSPowerDeliveryPacket *sourceCapabilities;
@property (readonly, nonatomic, strong) BSPowerDeliveryPDO *requestedPowerDeliveryObject;

+ (instancetype)RDOWithData:(NSData *)data sourceCapabilities:(BSPowerDeliveryPacket *)sourceCapabilities;

@end
