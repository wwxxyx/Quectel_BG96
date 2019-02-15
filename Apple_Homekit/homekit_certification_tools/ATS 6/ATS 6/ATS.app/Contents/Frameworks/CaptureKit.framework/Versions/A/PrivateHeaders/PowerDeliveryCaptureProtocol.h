//
//  PowerDeliveryCaptureProtocol.h
//  ATSMacApp
//
//  Created by Zachary Church on 3/28/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSCaptureProtocol.h>
#import <BusSniffer/BSiAPTypes.h>

@interface PowerDeliveryCaptureProtocol : ATSCaptureProtocol

@property (nonatomic, readonly, strong) NSString *captureProtocolName;

- (void)processData:(NSData *)data withCommand:(UInt8)command fromSource:(BSiAPSource)source atTimestamp:(UInt64)timestamp;

@end
