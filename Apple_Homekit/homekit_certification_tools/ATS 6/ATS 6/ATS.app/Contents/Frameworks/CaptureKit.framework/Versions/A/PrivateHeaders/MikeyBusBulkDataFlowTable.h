//
//  MikeyBusBulkDataFlowTable.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 10/3/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/BulkDataFlowTable.h>

@class ATSMikeyBusCaptureProtocol;
@class ATSMikeyBusBulkDataPacketEvent;

/**
 Bulk data flow table for iAP2 / iAP1 / HID / EA over MikeyBus bulk data channel.
 */
@interface MikeyBusBulkDataFlowTable : NSObject <BulkDataFlowTable>

/**
 Reference to Capture Protocol that owns this flow table.
 */
@property (nonatomic, weak) ATSMikeyBusCaptureProtocol *captureProtocol;

/**
 Returns a dictionary of Key and Values that represents parsed fields for MikeyBus Bulk Data Frames for the input bulk data event.
 
 @param event MikeyBus Bulk Data event.
 
 @return Returns a dictionary of Key and Values that represents parsed fields for MikeyBus Bulk Data Frames.
 */
- (NSDictionary <NSString *, NSString *> *)fieldsForBulkDataEvent:(ATSMikeyBusBulkDataPacketEvent *)event;

@end
