//
//  NMEAField.h
//  ATSMacApp
//
//  Created by Natália Cabral on 11/28/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef enum : UInt8 {
                   NMEAFieldStatusOk = 1,
                   NMEAFieldStatusEmpty,
                   NMEAFieldStatusInvalidFormat,
                   NMEAFieldStatusInvalidRange,
                   NMEAFieldStatusInvalidNumberOfDigits,
                   NMEAFieldStatusInvalidMinNumberOfDecimalDigits,
                   NMEAFieldStatusInvalidNumberOfDecimalDigits,
                   NMEAFieldStatusNotSupportedValue,
               } NMEAFieldStatus;

@interface NMEAField : NSObject

/*!
    The label of the field
 */
@property (nonatomic, strong) NSString *name;

/*!
    The string in the NMEA sentence for this field
 */
@property (nonatomic, strong) NSString *value;

/*!
    The actual value of the field after parsed
 */
@property (nonatomic, strong) id parsedValue;

/*!
    The status of the field
 */
@property (nonatomic) NMEAFieldStatus status;

/*!
    Initializes a new NMEAField with the specified name and value. 
    If the value is "", the field status is Empty. Otherwise, the status is Ok.
    During the parser, this status may change if the field can not be parsed.
 
    @param name     the field name
    @param value    the field value
    @return a new NMEAField
 */
- (id)initWithName:(NSString *)name value:(NSString *)value;

/*!
    Returns an error description if the field could not be properly parsed.
 
    @return the nmea field error description
 */
- (NSString *)errorDescription;

@end
