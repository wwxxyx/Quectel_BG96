//
//  ATFR3Event.h
//  ATSMacApp
//
//  Created by Marin Balde on 4/11/12.
//  Copyright © 2012 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/CaptureEvent.h>
#import <ATSR3BoxSupport/ATSR3BoxSupport.h>

@class ATFR3Frame;

@interface ATFR3Event : CaptureEvent {
    ATFR3Frame *_atfR3Frame;
}
@property (nonatomic, retain) ATFR3Frame *atfR3Frame;
@property (nonatomic, readonly) ATFR3Command command;
@property (nonatomic, readonly) NSError *error;

@property (nonatomic, readonly) NSNumber *numberValue;
@property (nonatomic, readonly) NSString *stringValue;
@property (nonatomic, readonly) const UInt8 *payloadBytes;

@end
