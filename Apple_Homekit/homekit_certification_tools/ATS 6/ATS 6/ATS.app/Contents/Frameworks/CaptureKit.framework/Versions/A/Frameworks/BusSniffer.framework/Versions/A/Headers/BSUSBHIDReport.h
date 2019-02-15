//
//  BSUSBHIDReport.h
//  BusSniffer
//
//  Created by Joe Basu on 5/7/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSUSBHIDTypes.h>
#import <BusSniffer/BSUSBTransfer.h>

@protocol BSUSBHIDReport <NSObject>

@property (nonatomic, readonly) BOOL hasReportID;
@property (nonatomic, readonly) UInt8 reportID;
@property (nonatomic, readonly, copy) NSData *data;
@property (nonatomic, readonly) UInt64 timestamp;
@property (nonatomic, readonly) BSUSBHIDReportType type;
@property (nonatomic, readonly) BSUSBHIDReportStatus status;
@property (nonatomic, readonly, retain) id <BSUSBTransfer> transfer;

@end

@interface BSUSBHIDReport : NSObject <BSUSBHIDReport> {
	BOOL _hasReportID;
	NSData *_data;
	UInt64 _timestamp;
	BSUSBHIDReportType _type;
	id <BSUSBTransfer> _transfer;
	BSUSBHIDReportStatus _status;
}

+ (id)usbHIDReportWithTimestamp:(UInt64)timestamp
						   type:(BSUSBHIDReportType)type
						   data:(NSData *)data
					hasReportID:(BOOL)hasReportID
						 status:(BSUSBHIDReportStatus)status
					   transfer:(id <BSUSBTransfer>)transfer;

- (id)initWithTimestamp:(UInt64)timestamp
				   type:(BSUSBHIDReportType)type
				   data:(NSData *)data
			hasReportID:(BOOL)hasReportID
				 status:(BSUSBHIDReportStatus)status
			   transfer:(id <BSUSBTransfer>)transfer;

@end
