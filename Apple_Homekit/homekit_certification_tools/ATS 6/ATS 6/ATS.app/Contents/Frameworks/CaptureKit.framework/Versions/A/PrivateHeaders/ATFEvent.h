//
//  ATFEvent.h
//  ATSMacApp
//
//  Created by Joe Basu on 12/7/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>
#import <ATSBoxSupport/ATSBoxSupport.h>

@interface ATFEvent : CaptureEvent {
    NSData *_data;
}

/*!
 @method    newEventForData:timestamp:
 @abstract  Factory method to instantiate and return the appropriate ATFEvent subclass (with retain count +1 to owner)
 */
+ (id)newEventForData:(NSData *)data timestamp:(UInt64)timestamp;

@property (nonatomic, readonly) UInt16 rawTimestamp;
@property (nonatomic, readonly) ATFSource source;
@property (nonatomic, readonly) ATFCommand command;
@property (nonatomic, readonly) UInt8 payload;
@property (nonatomic, readonly) UInt16 extendedPayload;

- (const UInt8 *)atfBytes;

@end
