//
//  BSUSBAnalyzerTransactionState.h
//  BusSniffer
//
//  Created by Joe Basu on 5/27/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import "BSUSBTypes.h"
#import "BSUSBTransfer.h"

enum {
	BSUSBTransactionRecognizerStateIdle,
	BSUSBTransactionRecognizerStateSetup,
	BSUSBTransactionRecognizerStateSetupData0,
	BSUSBTransactionRecognizerStateSetupRecognized,
	BSUSBTransactionRecognizerStateIn,
	BSUSBTransactionRecognizerStateInDataX,
	BSUSBTransactionRecognizerStateInRecognized,
	BSUSBTransactionRecognizerStateOut,
	BSUSBTransactionRecognizerStateOutDataX,
	BSUSBTransactionRecognizerStateOutRecognized,
	BSUSBTransactionRecognizerStatePing,
	BSUSBTransactionRecognizerStatePingRecognized
};

@interface BSUSBAnalyzerTransactionState : NSObject {
	NSUInteger _recognizerState;

	UInt8 _address;
	NSData *_data;
	UInt8 _endpoint;
	BOOL _isochronous;
	NSMutableArray *_packets;
	BSUSBTransactionStatus _status;
	UInt64 _timestamp;
	id <BSUSBTransfer> _transfer;
	BSUSBTransactionType _type;
	NSError *_error;
}

@property (nonatomic) NSUInteger recognizerState;

@property (nonatomic) UInt8 address;
@property (nonatomic, copy) NSData *data;
@property (nonatomic) UInt8 endpoint;
@property (nonatomic) BOOL isochronous;
@property (nonatomic, retain) NSMutableArray *packets;
@property (nonatomic) BSUSBTransactionStatus status;
@property (nonatomic) UInt64 timestamp;
@property (nonatomic, retain) id <BSUSBTransfer> transfer;
@property (nonatomic) BSUSBTransactionType type;
@property (nonatomic, retain) NSError *error;

- (void)reset;

@end
