//
//  BulkDataFlowTable.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 10/3/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ATSCaptureProtocol;
@class CaptureEvent;

@protocol BulkDataFlowTable <NSObject>

/**
 Create an instance of bulk data flow table using the capture protocol to be able to process the bulk data payload.
 
 @param captureProtocol Capture protocol that owns the bulk data flow table.
 */
+ (nonnull id <BulkDataFlowTable>)bulkDataFlowTableWithCaptureProtocol:(ATSCaptureProtocol *)captureProtocol;

/**
 Method to process bulk data payload from the capture event.
 
 @param event Capture event containing Bulk Data Payload.
 @param error is valid if there was an error processing the bulk data packet.
 
 @return Status on whether or not the processing the bulk data was successful.
 */
- (BOOL)handleBulkDataEvent:(CaptureEvent *)event withError:(NSError **)error;

/**
 Method to reset the bulk data flow table.
 */
- (void)reset;

NS_ASSUME_NONNULL_END

@end
