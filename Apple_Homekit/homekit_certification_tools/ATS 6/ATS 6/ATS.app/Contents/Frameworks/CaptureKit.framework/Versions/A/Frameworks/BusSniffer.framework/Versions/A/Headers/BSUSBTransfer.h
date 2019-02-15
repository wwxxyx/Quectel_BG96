//
//  BSUSBTransfer.h
//  BusSniffer
//
//  Created by Joe Basu on 2/27/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSUSBTypes.h>

@class BSUSBDeviceRequestInfo;

/*!
 @protocol BSUSBTransfer
 @abstract The BSUSBTransfer protocol defines common properties of USB transfers.
 */

@protocol BSUSBTransfer <NSObject>

@property (nonatomic, readonly) UInt8 address;
@property (nonatomic, readonly, copy) NSData *data;
@property (nonatomic, readonly) BSUSBTransferDirection direction;
@property (nonatomic, readonly) UInt8 endpoint;
@property (nonatomic, readonly) UInt64 timestamp;
@property (nonatomic, readonly) BSUSBTransferType type;
@property (nonatomic, readonly, copy) NSArray *transactions;
@property (nonatomic, readonly, copy) NSData *setupData; // only valid for control transfers
@property (nonatomic, readonly, retain) BSUSBDeviceRequestInfo *deviceRequestInfo; // only valid for control transfers
@property (nonatomic, readonly) BSUSBTransferStatus status;
@property (nonatomic, readonly) NSError *error;

@end

@interface BSUSBTransfer : NSObject <BSUSBTransfer> {
	UInt8 _address;
	NSData *_data;
	BSUSBTransferDirection _direction;
	UInt8 _endpoint;
	UInt64 _timestamp;
	BSUSBTransferType _type;
	NSData *_setupData;
	BSUSBTransferStatus _status;
	NSArray *_transactions;
	NSError *_error;
}

+ (id)usbTransferWithTimestamp:(UInt64)timestamp
						  type:(BSUSBTransferType)type
					   address:(UInt8)address
					  endpoint:(UInt8)endpoint
					 direction:(BSUSBTransferDirection)direction
					 setupData:(NSData *)setupData
						  data:(NSData *)data
						status:(BSUSBTransferStatus)status
				  transactions:(NSArray *)transactions
						 error:(NSError *)error;

- (id)initWithTimestamp:(UInt64)timestamp
				   type:(BSUSBTransferType)type
				address:(UInt8)address
			   endpoint:(UInt8)endpoint
			  direction:(BSUSBTransferDirection)direction
			  setupData:(NSData *)setupData
				   data:(NSData *)data
				 status:(BSUSBTransferStatus)status
		   transactions:(NSArray *)transactions
				  error:(NSError *)error;

@end
