//
//  ATSAppEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 7/27/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>

/*!
 @class         OutOfBandEvent
 @abstract      An event that lives outside of any capture device's timeframe.
 @discussion    Initializers do not need to provide a timestamp to super's init, as CaptureDocument will assign one
 */
@interface ATSAppEvent : CaptureEvent

/*!
 @method        eventWithInfo:
 @abstract      Creates an event from info dictionary
 @param         info
                Info dictionary, which should be the return of [serializeInto:] from an instance of OutOfBandEvent
 @discussion    This searches the runtime for an event with the desired class serialized into info. This should never be subclassed.
 */
+ (id)eventWithInfo:(NSDictionary *)info;

#pragma mark -
#pragma mark Serialization / Restoration

/*!
 @method    initWithInfo:
 @abstract  Initialize event with given info dictionary
 @param     info
            Info dictionary
 @return    Initialized event
 */
- (id)initWithInfo:(NSDictionary *)info;

/*!
 @method        serializeInto:
 @abstract      Encodes event into info
 @param         info
                Destination dictionary
 @dicussion     Subclassers must always call [super serializeInto:info]
 */
- (void)serializeInto:(NSMutableDictionary *)info;

@end
