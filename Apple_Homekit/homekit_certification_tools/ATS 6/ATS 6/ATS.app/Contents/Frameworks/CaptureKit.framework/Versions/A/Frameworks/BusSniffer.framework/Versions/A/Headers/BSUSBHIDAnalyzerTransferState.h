//
//  BSUSBHIDAnalyzerTransferState.h
//  BusSniffer
//
//  Created by Joe Basu on 6/13/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import "BSUSBTypes.h"

enum {
	BSUSBHIDInterruptTransferRecognizerStateIdle,
	BSUSBHIDInterruptTransferRecognizerStateWaitingForData,
};

@interface BSUSBHIDAnalyzerTransferState : NSObject {
	NSUInteger _recognizerState;
	UInt64 _timestamp;
	BSUSBTransferStatus _status;
	NSMutableArray *_transactions;
	NSData *_data;
	NSMutableData *_mutableData;
	UInt8 _address;
	UInt8 _endpoint;
	UInt8 _reportID;
	UInt16 _reportSize;
}

@property (nonatomic) NSUInteger recognizerState;
@property (nonatomic) UInt64 timestamp;
@property (nonatomic) BSUSBTransferStatus status;
@property (nonatomic, retain) NSMutableArray *transactions;
@property (nonatomic, copy) NSData *data;
@property (nonatomic, retain) NSMutableData *mutableData;
@property (nonatomic) UInt8 address;
@property (nonatomic) UInt8 endpoint;
@property (nonatomic) UInt8 reportID;
@property (nonatomic) UInt16 reportSize;

- (void)reset;

@end
