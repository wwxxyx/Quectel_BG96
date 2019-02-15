//
//  BSUSBTransaction.h
//  BusSniffer
//
//  Created by Joe Basu on 2/26/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSUSBTypes.h>
#import <BusSniffer/BSUSBTransfer.h>

/*!
 @protocol BSUSBTransaction
 @abstract The BSUSBTransaction protocol defines common properties of USB transactions.
 */

@protocol BSUSBTransaction <NSObject>

@property (nonatomic, readonly) UInt8 address;
@property (nonatomic, readonly, copy) NSData *data;
@property (nonatomic, readonly) UInt8 endpoint;
@property (nonatomic, readonly) BOOL isochronous;
@property (nonatomic, readonly, copy) NSArray *packets;
@property (nonatomic, readonly) BSUSBTransactionStatus status;
@property (nonatomic, readonly) UInt64 timestamp;
@property (nonatomic, readonly) BSUSBTransactionType type;
@property (nonatomic, readonly, copy) NSString *typeString;
@property (nonatomic, readonly) NSError *error;

@end

@interface BSUSBTransaction : NSObject <BSUSBTransaction> {
	UInt8 _address;
	NSData *_data;
	UInt8 _endpoint;
	BOOL _isochronous;
	NSArray *_packets;
	BSUSBTransactionStatus _status;
	UInt64 _timestamp;
	BSUSBTransactionType _type;
	NSError *_error;
}

+ (id)usbTransactionWithTimestamp:(UInt64)timestamp
							 type:(BSUSBTransactionType)type
						  address:(UInt8)address
						 endpoint:(UInt8)endpoint
					  isochronous:(BOOL)isochronous
							 data:(NSData *)data
						   status:(BSUSBTransactionStatus)status
						  packets:(NSArray *)packets
							error:(NSError *)error;

- (id)initWithTimestamp:(UInt64)timestamp
				   type:(BSUSBTransactionType)type
				address:(UInt8)address
			   endpoint:(UInt8)endpoint
			isochronous:(BOOL)isochronous
				   data:(NSData *)data
				 status:(BSUSBTransactionStatus)status
				packets:(NSArray *)packets
				  error:(NSError *)error;

@end
