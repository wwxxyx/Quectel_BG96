//
//  ATFR3Frame.h
//  ATSR3BoxSupport
//
//  Created by Marin Balde on 4/16/12.
//  Copyright © 2012 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <ATSR3BoxSupport/ATSR3BoxTypes.h>

extern NSString * const ATFR3FrameErrorDomain;

enum {
    ATFR3FrameErrorNoError = 0,
    ATFR3FrameErrorUnexpectedByte = 7000,
    ATFR3FrameErrorUnexpectedSOF,
    ATFR3FrameErrorUnknownPayloadSize,
    ATFR3FrameErrorDoubleEscape,
    ATFR3FrameErrorBufferOverflow,
    ATFR3FrameErrorPayloadSizeMismatch,
};
typedef NSInteger ATFR3FrameError;

typedef NS_ENUM(NSUInteger, ATFR3CaptureDeviceType) {
    ATFR3CaptureDeviceLightningBox = 0,
    ATFR3CaptureDeviceSmartConnectorBox = 1,
};

@interface ATFR3Frame : NSObject 

@property(nonatomic, readonly, assign) ATFR3Command command;
@property(nonatomic, readonly) NSString *commandName;
@property(nonatomic, readonly) NSData *rawData;
@property(nonatomic, readonly) NSData *payload;
@property(nonatomic, readonly) NSError *error;

@property (nonatomic, readonly) NSString *sourceDescription;
@property (nonatomic, readonly) NSString *commandDescription;
@property (nonatomic, readonly) NSString *payloadDescription;
@property (nonatomic, readonly) NSNumber *numberValue;
@property (nonatomic, readonly) NSString *stringValue;
@property (nonatomic, readonly) const UInt8 *payloadBytes;

+ (instancetype)frameWithCommand:(ATFR3Command)command
                     productName:(NSString *)productName
                       productID:(NSUInteger)productID
                        rawBytes:(const UInt8 *)rawBytes
                  rawBytesLength:(UInt8)rawBytesLength
                    payloadBytes:(const UInt8 *)payloadBytes
                   payloadLength:(UInt8)payloadLength
                       errorCode:(ATFR3FrameError)errorCode;

+ (instancetype)frameWithByte:(UInt8)byte
                  productName:(NSString *)productName
                    productID:(NSUInteger)productID
                    errorCode:(ATFR3FrameError)errorCode;

@end
