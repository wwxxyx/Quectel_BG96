//
//  BSPowerDeliveryPDO.h
//  BusSniffer
//
//  Created by Zachary Church on 3/29/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BSPowerDeliveryTypes.h"
#import "BSPowerDeliveryDataObject.h"

@interface BSPowerDeliveryPDO : BSPowerDeliveryDataObject

@property (readonly) BSPDRole role;
@property (readonly) BSPDPDOType type;

+ (instancetype)PDOWithData:(NSData *)data forRole:(BSPDRole)role;

@end
