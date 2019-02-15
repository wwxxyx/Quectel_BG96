//
//  ATSRFCOMMStream.h
//  CaptureKit
//
//  Created by Edgard Lima on 09/12/18.
//  Copyright (c) 2018 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

/** ATSPacketLoggerRFCOMStream instances represent streams of RFCOMM traffic detected in a source file. They encapsulate an ACL handle, an RFCOMM channel number, and the array of payloads that comprise the stream data. */
@interface ATSPacketLoggerRFCOMMStream : NSObject

/** The ACL handle used to identify the RFCOMM channel. */
@property uint16_t aclHandle;

/** The RFCOMM channel on which the iAP data appears. */
@property uint8_t rfcommChannel;

/** RFCOMM payloads containing iAP data. */
@property (copy) NSMutableArray *rfcommPayloads;

@end

/** ATSPacketLoggerRFCOMMPayload instances represent individual payloads of iAP data that comprise an RFCOMM data stream. */
@interface ATSPacketLoggerRFCOMMPayload : NSObject

/** A flag indicating whether the payload was from an outgoing ACL packet. */
@property BOOL isACLSend;

/** The timestamp for the source packet containing the RFCOMM payload. */
@property UInt64 timestamp;

/** The RFCOMM payload data. */
@property (copy) NSData *payload;

@end
