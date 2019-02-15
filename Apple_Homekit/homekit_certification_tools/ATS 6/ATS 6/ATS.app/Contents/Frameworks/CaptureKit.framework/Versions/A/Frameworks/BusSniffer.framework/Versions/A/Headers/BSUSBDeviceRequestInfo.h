//
//  BSUSBDeviceRequestInfo.h
//  BusSniffer
//
//  Created by Joe Basu on 3/31/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSUSBTypes.h>
#import <BusSniffer/BSUSBHIDTypes.h>

@interface BSUSBDeviceRequestInfo : NSObject {
	BSUSBDeviceRequestRecord _deviceRequestRecord;
}

+ (id)deviceRequestInfoWithDeviceRequestData:(NSData *)deviceRequestData;
- (id)initWithDeviceRequestData:(NSData *)deviceRequestData;

@property (nonatomic, readonly) BSUSBDeviceRequestRecord deviceRequestRecord;
@property (nonatomic, readonly) BSUSBDeviceRequestDirection direction;
@property (nonatomic, readonly) BSUSBDeviceRequestType type;
@property (nonatomic, readonly) BSUSBDeviceRequestRequest request;
@property (nonatomic, readonly) UInt16 length;
@property (nonatomic, readonly) BSUSBDeviceRequestRecipient recipient;

/*!
 @property interfaceNumber
 @abstract The interface number for the request. This property's value is only valid for a requests in which wIndex is "Interface".
 */
@property (nonatomic, readonly) UInt8 interfaceNumber;

/*!
 @property descriptorTypeRequested
 @abstract The descriptor type requested. This property's value is only valid for a Get Descriptor or Set Descriptor request.
 */
@property (nonatomic, readonly) BSUSBDescriptorType descriptorTypeRequested;


/*!
 @property descriptorIndexRequested
 @abstract The descriptor index requested. This property's value is only valid for a Get Descriptor or Set Descriptor request.
 */
@property (nonatomic, readonly) UInt8 descriptorIndexRequested;

/*!
 @property reportType
 @abstract The report type requested. This property's value is only valid for a Get Report or Set Report request in the HID class.
 */
@property (nonatomic, readonly) BSUSBHIDReportType reportType;

/*!
 @property reportID
 @abstract The report ID requested. This property's value is only valid for a Get Report or Set Report request in the HID class.
 */
@property (nonatomic, readonly) UInt8 reportID;

@end
