//
//  BluetoothCaptureTransport+Parsing.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/3/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/BluetoothCaptureTransport.h>
#import <ATSImportKit/IKBluetoothAclImportPacket.h>
#import <ATSImportKit/IKBluetoothHciContainerImportPacket.h>

@class FTDataFrame;
@class ComprobeFrame;

@interface BluetoothCaptureTransport (Parsing)

/**
 Prepares the given Bluetooth frame for parsing and display.
 
 @param frame The new Bluetooth frame to parse.
 @param timestamp The normalized timestamp to use when displaying the frame.
 */
- (void)processFrame:(FTDataFrame *)frame atTimestamp:(UInt64)timestamp;

/*!
 @method    processComprobeFrameData:fromAddress:atTimestamp:
 @abstract  Processes comprobe frame data.
 */
- (void)processComprobeFrameData:(NSData *)data fromAddress:(NSData *)address timestamp:(UInt64)timestamp;

/*!
 @method    processRFCOMMPayload:fromSource:atTimestamp:
 @abstract  Process RFCOMM payload and return relevant
 */
- (void)processRFCOMMPayload:(NSData *)payload fromSource:(BSiAPSource)source timestamp:(UInt64)timestamp;

/*!
 Process the L2CAP payload and delivery a L2CAP event
 @param payload an object NSData that contains the payload
 @param source indicates the source (Apple Device or Accessory)
 @param timestamp the timestamp of origin data
 */
- (void)processL2CAPPayload:(NSData *)payload fromSource:(BSiAPSource)source timestamp:(UInt64)timestamp;

/*!
 Process the ACL packet and delivery a ComProbe frame event
 @param aclImportPacket an object IKBluetoothAclImportPacket that contains the payload and frame data
 @param source indicates the source (Apple Device or accessory)
 @param timestamp the timestamp of origin data
 */
- (void)processAclImportPacket:(IKBluetoothAclImportPacket*)aclImportPacket fromSource:(BSiAPSource)source timestamp:(UInt64)timestamp;

/*!
 Process the HCI packet and delivery a HciContainer event
 @param hciContainerImportPacket an object IKBluetoothHciContainerImportPacket that contains the payload
 @param timestamp the timestamp of origin data
 */
- (void)processHciContainerImportPacket:(IKBluetoothHciContainerImportPacket*)hciContainerImportPacket timestamp:(UInt64)timestamp;

@end
