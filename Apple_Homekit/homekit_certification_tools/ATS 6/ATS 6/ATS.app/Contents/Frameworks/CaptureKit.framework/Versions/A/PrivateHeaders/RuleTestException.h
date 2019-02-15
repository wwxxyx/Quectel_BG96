//
//  RuleTestException.h
//  ATS
//
//  Created by Joe Basu on 8/9/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>

/*!
 @const     RuleTestAssertExceptionErrorException
 @abstract  Thrown when the RT_ASSERT_EXCEPTION macro's asserted exception was not seen.
 */
extern NSString *const RuleTestAssertExceptionErrorException;

/*!
 @const     RuleTestNotAUSBNativeHIDReportDescriptorTransferEventException
 @abstract  Thrown when the emulation code is expecting to return a USBNativeHIDReportDescriptorTransferEvent
            but it's not found.
 */
extern NSString *const RuleTestNotAUSBNativeHIDReportDescriptorTransferEventException;

/*!
 @const     RuleTestNotAUSBNativeHIDReportTransferEventException
 @abstract  Thrown when the emulation code is expecting to return a USBNativeHIDReportTransferEvent
            but it's not found.
 */
extern NSString *const RuleTestNotAUSBNativeHIDReportTransferEventException;

@interface RuleTestException : NSException {
}

/*!
 @macro     RT_ASSERT_EXCEPTION
 @abstract  Macro to assert that a particular exception was thrown for a given piece of code.
 @param     shouldAssert
            Whether the exception is expected or not.
 @param     exceptionName
            The name of the exception that is being asserted for.
 @param     code
            The piece of code to run which may or may not cause the exception.
 */
#define RT_ASSERT_EXCEPTION(shouldAssert, exceptionName, code)                                                                                    \
    {                                                                                                                                             \
        if ((shouldAssert)) {                                                                                                                     \
            BOOL caught = NO;                                                                                                                     \
            @try {                                                                                                                                \
                (code);                                                                                                                           \
            }                                                                                                                                     \
            @catch (NSException * exception)                                                                                                      \
            {                                                                                                                                     \
                caught = [exception.name isEqualToString:(exceptionName)];                                                                        \
            }                                                                                                                                     \
            if (caught) {                                                                                                                         \
                [self assertParserErrorSeen];                                                                                                     \
                                                                                                                                                  \
            } else {                                                                                                                              \
                [RuleTestException raise:RuleTestAssertExceptionErrorException format:@"could not find asserted exception: %@", (exceptionName)]; \
            }                                                                                                                                     \
        } else {                                                                                                                                  \
            (code);                                                                                                                               \
        }                                                                                                                                         \
    }

@end
