//
//  ATSIDBusBulkDataFlowTable.h
//  ATSMacApp
//
//  Created by Zachary Church on 6/11/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/IDBusCaptureProtocol.h>

@class CaptureTransport;
@class IDBusPacketEvent;

@interface ATSIDBusBulkDataFlowTable : NSObject

+ (id)flowTableWithCaptureProtocol:(IDBusCaptureProtocol *)captureProtocol;
- (void)handleIDBusEvent:(IDBusPacketEvent *)event;
- (void)reset;

@end
