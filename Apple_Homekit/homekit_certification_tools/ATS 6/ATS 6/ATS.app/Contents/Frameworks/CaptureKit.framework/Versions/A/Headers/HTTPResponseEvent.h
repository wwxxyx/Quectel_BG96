//
//  HTTPResponseEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/16/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>
#import <BusSniffer/BusSniffer.h>

@class APHTTPResponse;
@class HTTPRequestEvent;

@interface HTTPResponseEvent : CaptureEvent

/*!
 @property  request
 @abstract  The request which triggered this response
 */
@property (nonatomic, retain) HTTPRequestEvent *request;

/*!
 @property  source
 @abstract  The source of the request
 */
@property (nonatomic, assign) BSiAPSource source;

/*!
 @property  response
 @abstract  The HTTP response object
 */
@property (nonatomic, retain) APHTTPResponse *response;

/*!
 Returns the parsed content object, if any
 */
@property (nonatomic, readonly) NSDictionary *parsedContent;

/*!
 The error string (localized description) if content parsing as plist fails
 
 @note Not every HTTP payload is a plist. The parse error is valid assuming that this HTTP response could have contained a plist. It's up to the higher layer protocol to make the decision of whether or not the content is expected as plist.
 */
@property (nonatomic, readonly) NSError *contentPlistParseError;

/**
 Instantiate HTTP response event
 */
- (instancetype)initWithTimestamp:(UInt64)timestamp
                         response:(APHTTPResponse *)response
                     requestEvent:(HTTPRequestEvent *)requestEvent
                           source:(BSiAPSource)source;

@end
