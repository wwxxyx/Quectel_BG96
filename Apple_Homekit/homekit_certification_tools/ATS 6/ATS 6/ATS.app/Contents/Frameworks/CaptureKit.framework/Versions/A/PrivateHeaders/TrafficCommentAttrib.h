//
//  TrafficCommentAttrib.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 7/21/11.
//  Copyright 2011 Apple Inc. All rights reserved.
//

#import <CaptureKit/TrafficAttribute.h>
#import <Cocoa/Cocoa.h>

@interface TrafficCommentAttrib : TrafficAttribute {
    NSString *_comment;
}
@property (nonatomic, retain) NSString *comment;

- (id)initWithTimestamp:(UInt64)aTimestamp;
- (id)initWithInfo:(NSDictionary *)info;
- (NSMutableDictionary *)exportedInfo;

@end

@interface TrafficCommentCell : NSTextFieldCell
@end
