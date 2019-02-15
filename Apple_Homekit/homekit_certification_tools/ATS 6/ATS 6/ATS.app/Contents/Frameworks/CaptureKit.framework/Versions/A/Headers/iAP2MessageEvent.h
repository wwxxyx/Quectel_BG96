//
//  iAP2MessageEvent.h
//  ATSMacApp
//
//  Created by Joe Basu on 4/2/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/iAP2Event.h>
#import <CaptureKit/CaptureKit.h>
// To use the I2MMessage class, import iAP2MessageKit.I2MMessage
@class I2MMessage;

NS_ASSUME_NONNULL_BEGIN

@interface iAP2MessageEvent : iAP2Event<ReportAnalyzerDisplayProtocol>

@property (nonatomic, retain) I2MMessage *message;
@property (nonatomic, retain, nullable) NSError *error;

@end

NS_ASSUME_NONNULL_END
