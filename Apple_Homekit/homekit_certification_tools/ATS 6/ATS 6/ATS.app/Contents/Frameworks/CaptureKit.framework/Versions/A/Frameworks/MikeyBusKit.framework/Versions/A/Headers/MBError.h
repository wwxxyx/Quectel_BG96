//
//  MBError.h
//  MikeyBusKit
//
//  Created by Wesley McCloy on 10/15/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, MBBulkDataPacketDirectionType);

/*!
 @enum          MBErrors
 @discussion    This enum lists all error codes
 */
typedef NS_ENUM(NSInteger, MBErrorCode) {
    MBErrorCodeUndefined = 0,
    
    // Superframes
    MBErrorCodeSFInvalidError,
    MBErrorCodeSFSODPError,
    MBErrorCodeSF8B10BError,
    MBErrorCodeSFNoChannelForSlot,
    
    // Broadcast frames
    MBErrorCodeBFUnexpectedLengthError,
    MBErrorCodeBFInvalidSOSError,
    MBErrorCodeBFInvalidCRCError,
    
    // Bulk data packets
    MBErrorCodeBDFInvalidSOSError,
    MBErrorCodeBDFNoPayloadError,
    MBErrorCodeBDFPayloadLengthMismatchError,
    
    // Designator Packets
    MBErrorCodeDesignatorUnexpectedPacketLength,
    MBErrorCodeDesignatorReceivedRetForNoGet,
    MBErrorCodeDesignatorUnknownConfiguration,
    MBErrorCodeDesignatorUnexpectedFunctionGroupID,
    
    // L05 Payloads
    MBErrorCodeL05UnexpectedLengthError
};

/*!
 @class         MBError
 @discussion    This header/class defines the error domain to be used by this project as well as convenience methods
 */
@interface MBError : NSError

/**
 Defines the Error Domain of this project

 @returns "LightningAudioErrorDomain"
 */
+ (NSString *)domain;

/**
 Returns an MBError for an SODP error on the Superframe
 
 @param invalidSODP: the non-SODP symbol
 */
+ (instancetype)errorSFSODPErrorWithData:(uint16_t)invalidSODP;

/**
 Returns an MBError for an 8B10B error
 
 @param invalid8B10B: the symbol that failed to decode
 */
+ (instancetype)errorSF8B10BErrorWithData:(uint16_t)invalid8B10B;

/**
 Returns an MBError for when data was received for a slot that was not allocated in the TCA
 
 @param slot: The slot that was not allocated
 @param frame: The frame where this happened
 @param phase: The phase of the given slot (since the slot index resets for each phase)
 */
+ (instancetype)errorSFNoChannelForSlot:(uint16_t)slot forData:(uint16_t)data atFrame:(uint16_t)frame duringPhase:(MBBulkDataPacketDirectionType)phase;

/**
 Returns error with the given code, adding userInfo dict based on code.

 @param errorCode: MBErrors enum type that defines the error
 @returns The appropriate NSError with localization strings
 */
+ (instancetype)errorWithErrorCode:(MBErrorCode)errorCode;


@end
