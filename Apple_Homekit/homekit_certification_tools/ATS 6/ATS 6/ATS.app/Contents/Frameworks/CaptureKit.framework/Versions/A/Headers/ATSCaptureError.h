//
//  ATSCaptureError.h
//  ATSMacApp
//
//  Created by Wesley McCloy on 2/6/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

/** 
 Error domain for CaptureError subclass of NSError
 */
extern NSString *const ATSCaptureErrorDomain;

/**
 Defines the different supported types of CaptureErrors
 */
typedef NS_ENUM(NSInteger, ATSCaptureErrorCode) {
    /// Unknown Capture Error
    ATSCaptureErrorCodeUnknown = 0,
    /// Error when ATS suspects that the AirPlay Diagnostic Mode profile has not been installed. This causes traffic to be unparsable.
    ATSCaptureErrorCodeWiFiSniffingProfileDisabled = 1,
    /// Error when ATS suspects that the AirPlay Diagnostic Mode profile has not been installed. This causes traffic to be unparsable.
    ATSCaptureErrorCodeAirPlayWiFiSniffingProfileDisabled = 2,
};

/**
 This subclass of NSError helps strictly identify errors that arise from problems with the capture.
 The different errors are represented in the CaptureErrorCode enum.
 */
@interface ATSCaptureError : NSError

/**
 Class instance method to create a new CaptureError given the code. Handles the
 domain and localizedDescription of NSError
 
 @param code The CaptureErrorCode that identifies the error
 @returns The new CaptureError
 */
+ (instancetype)errorWithCode:(ATSCaptureErrorCode)code;

/**
 Intializes the new CaptureError given the error code. Handles the
 domain and localizedDescription of NSError
 
 @param code The CaptureErrorCode that identifies the error
 @returns The initalized CaptureError
 */
- (instancetype)initWithCode:(ATSCaptureErrorCode)code;

/**
 The name of the CaptureError.
 */
@property (readonly) NSString *name;

/**
 The error's code casted to a CaptureErrorCode for explicit comparisons
 */
@property (readonly) ATSCaptureErrorCode captureErrorCode;

@end
