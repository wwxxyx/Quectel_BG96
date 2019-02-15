//
//  BSUSBTransferRecognizerState.h
//  BusSniffer
//
//  Created by Joe Basu on 5/27/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import "BSUSBTypes.h"

enum {
	BSUSBTransferRecognizerStateIdle,
	BSUSBTransferRecognizerStateControlWaitingForDataIn,
	BSUSBTransferRecognizerStateControlWaitingForDataOut,
	BSUSBTransferRecognizerStateControlWaitingForDataOutPingAck,
	BSUSBTransferRecognizerStateControlWaitingForStatusIn,
	BSUSBTransferRecognizerStateControlWaitingForStatusOut,
	BSUSBTransferRecognizerStateControlWaitingForStatusOutPingAck
};

@class BSUSBTransfer;
@class BSUSBDeviceRequestInfo;

@interface BSUSBAnalyzerTransferState : NSObject {
	NSUInteger _recognizerState;
	UInt64 _timestamp;
	BSUSBTransferDirection _direction;
	BSUSBTransferStatus _status;
	NSMutableArray *_transactions;
	NSData *_data;
	NSMutableData *_mutableData;
	NSData *_setupData;
	UInt8 _address;
	UInt8 _endpoint;
	NSError *_error;
	
	// control transfer state variables
	BSUSBDeviceRequestInfo *_controlTransferDeviceRequestInfo;
	UInt16 _controlTransferBytesExpected;
	UInt16 _controlTransferBytesPending;
	UInt8 _controlTransferRequestDirection;
	
}

@property (nonatomic) NSUInteger recognizerState;
@property (nonatomic) UInt64 timestamp;
@property (nonatomic) BSUSBTransferDirection direction;
@property (nonatomic) BSUSBTransferStatus status;
@property (nonatomic, retain) NSMutableArray *transactions;
@property (nonatomic, copy) NSData *data;
@property (nonatomic, retain) NSMutableData *mutableData;
@property (nonatomic, copy) NSData *setupData;
@property (nonatomic) UInt8 address;
@property (nonatomic) UInt8 endpoint;
@property (nonatomic, retain) NSError *error;


@property (nonatomic, retain) BSUSBDeviceRequestInfo *controlTransferDeviceRequestInfo;
@property (nonatomic) UInt16 controlTransferBytesExpected;
@property (nonatomic) UInt16 controlTransferBytesPending;
@property (nonatomic) UInt8 controlTransferRequestDirection;

- (void)reset;

@end
