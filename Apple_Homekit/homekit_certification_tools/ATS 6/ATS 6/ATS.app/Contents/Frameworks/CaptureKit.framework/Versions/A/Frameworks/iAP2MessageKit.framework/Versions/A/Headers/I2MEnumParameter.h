//
//  I2MEnumParameter.h
//  iAP2MessageKit
//
//  Created by Joe Basu on 5/23/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import "I2MParameter.h"

/*!
 @class     I2MEnumParameter
 @abstract  A flexible record-style class representing an instance of an iAP2 enum parameter
 */

@interface I2MEnumParameter : I2MParameter

/*!
 @property valueDescription
 @abstract A string description of the enum value
 */
@property (nonatomic, copy) NSString *valueDescription;

@end
