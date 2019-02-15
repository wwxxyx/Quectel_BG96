//
//  IDBusCaptureProtocol.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 7/19/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSCaptureProtocol.h>

@interface IDBusCaptureProtocol : ATSCaptureProtocol

@property (nonatomic, readonly, strong) NSString *captureProtocolName;

/*!
 Map of IDBus endpoints to it's function map that is received in Bulk Data Endpoint Info Response events. Example: "0xb3 Bulk Data Endpoint Info Response", "endpoint: 0x00, function: HID". This context is later used to set function numbers in bulk data packets that conform to IDBPacketBulkDataPayloadTransferring protocol.
 */
@property (nonatomic, strong, readonly) NSDictionary <NSNumber *, NSNumber *> *endpointToFunctionMap;


/*!
 @method    processByte:timestamp:
 @abstract  Processes one ID Bus byte
 */
- (void)processByte:(UInt8)byte timestamp:(UInt64)timestamp;

/*!
 @method    processBreakAtTimestamp:
 @abstract  Processes an ID Bus break
 */
- (void)processBreakAtTimestamp:(UInt64)timestamp;

/*!
 @method    processWakeAtTimestamp:
 @abstract  Processes an ID Raw Wake
 */
- (void)processWakeAtTimestamp:(UInt64)timestamp;

/*!
 @method    reset
 @abstract  Resets the protocol state. Should be called, for example, when an accessory is disconnected and reconnected.
 */
- (void)reset;

@end
