//
//  BSPowerDeliveryDataObject.h
//  BusSniffer
//
//  Created by Zachary Church on 3/30/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BSPowerDeliveryTypes.h"

@interface BSPowerDeliveryDataObject : NSObject

@property (readonly, strong) NSData *data;
@property (readonly, strong) NSError *error;
@property (readonly, strong) NSString *localizedTypeName;
@property (readonly, strong) NSArray *fieldTypes;

/**
 Whether or not the externally powered bit is set. Externally powered bit can only be present in a PDO with SourceCapabilities type.
 */
@property (nonatomic, readonly) BOOL isExternallyPowered;

- (NSDictionary *)localizedFieldValuesForDisplay;
- (UInt16)fieldAtBitOffset:(NSUInteger)offset withBitLength:(NSUInteger)length;
- (UInt16)valueForField:(BSPDDataObjectField)field;

@end
