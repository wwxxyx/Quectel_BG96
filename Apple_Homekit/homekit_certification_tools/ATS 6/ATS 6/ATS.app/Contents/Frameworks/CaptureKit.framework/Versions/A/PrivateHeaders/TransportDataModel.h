//
//  TranportDataModel.h
//  ATSMacApp
//
//  Created by Carlos Frederico Azevedo on 4/14/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/ProductPlanTypes.h>

/*!
 @class      TransportDataModel
 @abstract   This abstract class describes the basic information shared by all instances of possible
             transports for the accessory.
 */
@interface TransportDataModel : NSObject <NSCoding>

/*!
 @property  transportType
 @abstract  Returns the transport type of this object.
 */
@property (readonly, nonatomic) TransportType transportType;

/*!
 @property  powerSource
 @abstract  The PowerSourceType of the transport.
 */
@property (readonly, nonatomic) USBVbusPowerSourceType powerSource;

/*!
 @property  name
 @abstract  The transport's name for reporting purposes.
 */
@property (readonly, nonatomic) NSString *name;

/*!
        Returns YES if the transport is of type USBTransport or USBAndUARTTransport.
 */
@property (readonly, nonatomic) BOOL isUSBTransport;

/*!
 @method    transportFromType
 @abstract  Factory method to create transport instances by the given type.
 @param     type
            The transport type for the new instance.
 */
+ (TransportDataModel *)transportFromType:(TransportType)type;

@end
