//
//  CaptureEvent.h
//  ATSMacApp
//
//  Created by Zachary Church on 8/1/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//
// https://gitlab.sd.apple.com/ats/CaptureKitDocumentation

#import <Foundation/Foundation.h>

@class CaptureTransport;
@class ParseError;

/** An enumeration of the types of Event status. */
typedef NS_ENUM(NSInteger, ATSEventStatus) {
    ATSEventStatusNull = 0,        ///< Nil
    ATSEventStatusSuccess,         ///< Green Check
    ATSEventStatusWarning,         ///< Yellow Exclamation
    ATSEventStatusError,           ///< Red Exclamation
    ATSEventStatusInfo,            ///< No Image
};

@interface CaptureEvent : NSObject

/**
 The hardware timestamp of the capture event. Source is the capture hardware.
 */
@property (nonatomic) UInt64 timestamp;

/**
 Derived timestamp from the hardware timestamp and the system time (per capture transport).
 
 @note Set during the time of delivery through the Capture Transport (captureSystemTimestampForEvent:).
 */
@property (nonatomic) UInt64 absoluteTimestampSince1970;

@property (nonatomic) UInt64 uniqueID;

/*!
 String the filtering is performed over
 */
@property (nonatomic, retain) NSString *filterTextCache;

/*!
 Transport assigned by EventBroker which can be used to figure out where the timestamp came from
 */
@property (nonatomic, assign) CaptureTransport *transport;

/*!
 A reference to related previous CaptureEvent layer event or none if it has no related event.
 
 @discussion As each layer is parsed, it generates the corresponding event and passes the event’s reference so that the next layer can assign previousLayer and nextLayer appropriately.
 
 This connection between layers (events) make it possible to navigate between them and also to show the immediate Layer tree in the Details pane for all views (or events) that implement it.
 */
@property (nonatomic, strong) NSArray<CaptureEvent *> *previousLayerEvents;

/*!
 A reference to related next CaptureEvent layer event or none if it has no related event.
*/
@property (nonatomic, strong) NSArray<CaptureEvent *> *nextLayerEvents;

/*!
 @property      suitableForRuleEngine
 @abstract      Whether or not the event should be delivered to a rule engine. The default is YES
 @discussion    iAPPackets with errors are delivered to TVCs but not the rule engine
 */
@property (nonatomic, assign) BOOL suitableForRuleEngine;

/*!
 Status of the capture event.
 
 @note used to determine the color of the event row.
 */
@property (nonatomic, readonly) ATSEventStatus status;

+ (instancetype)eventWithTimestamp:(UInt64)timestamp;
+ (instancetype)eventWithTimestamp:(UInt64)timestamp suitableForRuleEngine:(BOOL)suitableForRuleEngine;
- (instancetype)initWithTimestamp:(UInt64)timestamp;
- (instancetype)initWithTimestamp:(UInt64)timestamp suitableForRuleEngine:(BOOL)suitableForRuleEngine;

- (NSString *)briefDescription;

/*!
 method which returns a dictionary with identifier fields of this event, uniqueID, timestamp and className
 @return the fields required to uniquely identify this event.
 */
- (NSMutableDictionary *)dictionaryDescription;

/*!
 method which returns the values of dictionaryDescription as well as all subscribers to this eventClass according to EventBroker
 @return the same dictionary as dictionaryDescription as well as all subscribers to this eventClass according to EventBroker
 */
- (NSMutableDictionary *)dictionaryDescriptionWithSubscribers:(NSArray *)subscribers;

/**
 Adds an error (created by each distinct parser) as a ParseError by supplying:
 
 @param error The NSError parse error created by the specific protocol
 @param identifier Supply an identifier if the error's userInfo doesn't have an "identifier" key/value
 @param identificationKeys The set of keys that identify the occurence of the parse error
 @return The added ParseError
 */
- (ParseError *)addErrorAsParseError:(NSError *)error withIdentifier:(NSString *)identifier andIdentificationKeys:(NSSet<NSString *> *)identificationKeys;

/**
 An array of parse errors for this capture event.
 */
@property (nonatomic, readonly) NSArray<ParseError*> *parseErrors;

@end
