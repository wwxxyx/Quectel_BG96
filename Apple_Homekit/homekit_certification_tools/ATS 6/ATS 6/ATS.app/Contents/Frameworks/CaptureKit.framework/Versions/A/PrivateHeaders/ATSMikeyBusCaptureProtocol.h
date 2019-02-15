//
//  ATSMikeyBusCaptureProtocol.h
//  ATSMacApp
//
//  Created by Wesley McCloy on 12/7/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSCaptureProtocol.h>

@class MikeyBusBulkDataFlowTable;

/**
 This CaptureProtocol subclass if for the MikeyBus protocol. It handles the packetizers and then delivers
 the events to the CaptureTransport.
 */
@interface ATSMikeyBusCaptureProtocol : ATSCaptureProtocol

/**
 Reference to MikeyBus Bulk Data Flow table.
 */
@property (nonatomic, strong) MikeyBusBulkDataFlowTable *mikeyBusBulkDataFlowTable;

/**
 This is the main interface for actually sending the data to a parser.
 Called by the CaptureTransport
 
 @param shortInt a 16 bit (encoded) piece of data

 maybe this should just be an NSData and we should handle that at the lowest level?
 */
- (void)processShort:(uint16_t)shortInt;

@end
