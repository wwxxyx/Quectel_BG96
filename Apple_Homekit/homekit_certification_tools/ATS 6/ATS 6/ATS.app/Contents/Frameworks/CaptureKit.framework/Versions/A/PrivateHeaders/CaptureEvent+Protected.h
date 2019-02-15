//
//  CaptureEvent+Protected.h
//  ATSMacApp
//
//  Created by Marin Balde on 8/27/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>
#import <CaptureKit/DisplayProtocols.h>
#import <CaptureKit/TriggerEventRuleClassification.h>
#import <CaptureKit/EventChanneling.h>
#import <AppKit/AppKit.h>
#import <CaptureKit/ATSConstants.h>

/** The domain for missing packet errors. */
extern NSString * const ATSMissingPacketErrorDomain;

/** An enumeration of the types of errors encapsulated by missing packet error events. */
typedef NS_ENUM(NSInteger, ATSMissingPacketError) {
    /** Packet not seen in traffic stream; assumed missing. */
    ATSMissingPacketAssumedError = 1,
};

/** An enumeration of possible error levels for messages inserted into capture document. */
typedef NS_ENUM(NSInteger, MessageEventSeverity) {
    /** The message is an informative annotation. */
    MessageEventSeverityInfo = 0,
    
    /** The message is a warning. */
    MessageEventSeverityWarning,
    
    /** The message indicates an error. */
    MessageEventSeverityError,
};

/*!
 @macro     WITH_EVENT_TRIGGER
 @abstract  Helps to classify an event as essential or nonessential
 @example   This is an essential event classification: WITH_EVENT_TRIGGER(iAP2MessageEvent)
 @example   This is a non-essential event classification: WITH_EVENT_TRIGGER(iAP2MessageEvent, kEventIsNotEssential)
 */
#define kEventIsNotEssential NO
#define CONSTRUCT_WITH_EVENT_TRIGGER(className, typeObject, isEssential, ...) [TriggerEventRuleClassification eventTriggerClassificationForEventClass:[className class] type:typeObject essential:isEssential]

#define WITH_EVENT_TRIGGER(className, ...) CONSTRUCT_WITH_EVENT_TRIGGER(className, nil, ##__VA_ARGS__, YES)

#define CONSTRUCT_ACTIVATOR_WITH_EVENT(className, typeObject, isEssential, ...) [TriggerEventRuleClassification eventTriggerClassificationForEventClass:[className class] type:typeObject essential:isEssential activate:YES]

#define ACTIVATE_WITH_EVENT(className, ...) CONSTRUCT_ACTIVATOR_WITH_EVENT(className, nil, ##__VA_ARGS__, YES)


@interface CaptureEvent () <CommonDisplay, iAPPacketDisplay, USBPacketDisplay, USBTransactionDisplay, USBTransferDisplay, USBHIDReportDisplay, ATFR3Display, IDBusDisplay, iAP2LinkPacketDisplay, iAP2MessageDisplay, ATFDisplay, iAP2FileTransferDatagramDisplay>

@property (nonatomic) BOOL multiline;
@property (nonatomic) CGFloat rowHeight;

/**
 Suggested color of the row that displays this CaptureEvent in traffic view.
 */
@property (nonatomic, readonly) NSColor *rowTextColor;

/**
 Channel this instance of the event belongs to.
 */
@property (nonatomic, strong, readwrite) id<EventChanneling> eventChannel;

- (DetailsViewTree *)detailsViewTreeDescription;

/*!
 @method    typeClassification
 @abstract  Classification which describes the type of event this is within this class of event. May return nil
 */
- (id)typeClassification;

@end
