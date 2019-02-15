//
//  MessageEvent.h
//  ATSMacApp
//
//  Created by Marin Balde on 8/27/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>
#import <CaptureKit/DisplayProtocols.h>

typedef NS_ENUM(NSInteger, MessageEventSeverity); // Forward declaring the enum

@interface MessageEvent : CaptureEvent <iAPPacketDisplay, USBPacketDisplay, USBTransactionDisplay, USBTransferDisplay, USBHIDReportDisplay, iAP2MessageDisplay, ReportAnalyzerDisplayProtocol>

@property (nonatomic, copy) NSString *message;
@property (nonatomic, copy) NSString *source;
@property (nonatomic) MessageEventSeverity severity;
/// This is an optional feature where you can link the message to an other event
@property (nonatomic, weak) CaptureEvent *relatedEvent;

+ (id)messageEventWithTimestamp:(UInt64)timestamp message:(NSString *)message;
+ (id)messageEventWithTimestamp:(UInt64)timestamp source:(NSString *)source message:(NSString *)message;
+ (id)messageEventWithTimestamp:(UInt64)timestamp source:(NSString *)source message:(NSString *)message severity:(MessageEventSeverity)severity;

- (id)initWithTimestamp:(UInt64)timestamp message:(NSString *)message;
- (id)initWithTimestamp:(UInt64)timestamp source:(NSString *)source message:(NSString *)message;
- (id)initWithTimestamp:(UInt64)timestamp source:(NSString *)source message:(NSString *)message severity:(MessageEventSeverity)severity;

@end
