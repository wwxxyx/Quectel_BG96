//
//  RuleTests+HTTP.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/22/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/RuleTests.h>

#import <CaptureKit/HTTPRequestEvent.h>
#import <CaptureKit/HTTPResponseEvent.h>

@class RTEmulatedTCPConnection;

@interface RuleTests (HTTP)

/*!
 @method    emulateHTTPRequestOnConnection:requestLine:header:content:
 @abstract  Emulates an HTTP request with the request line (ex. @"GET /index.html HTTP/1.1"), header, and optional content
 */
- (HTTPRequestEvent *)emulateHTTPRequestOnConnection:(RTEmulatedTCPConnection *)connection requestLine:(NSString *)requestLine header:(NSDictionary *)header content:(NSData *)content;

/*!
 @method        emulateHTTPResponseOnConnection:statusLine:header:content:
 @abstract      Emulates an HTTP response with the status line (ex. @"HTTP/1.1 200 OK"), header, and content
 */
- (HTTPResponseEvent *)emulateHTTPResponseOnConnection:(RTEmulatedTCPConnection *)connection statusLine:(NSString *)statusLine header:(NSDictionary *)header content:(NSData *)content;

#pragma mark -
#pragma mark Convenience

/*!
 @method    emulateHTTPBinaryPlistRequestOnConnection:requestLine:header:plist:
 @abstract  Emulates a bplist request, setting the Content-Length appropriately and the Content-Type to @"application/x-apple-binary-plist"
 */
- (HTTPRequestEvent *)emulateHTTPBinaryPlistRequestOnConnection:(RTEmulatedTCPConnection *)connection requestLine:(NSString *)requestLine header:(NSDictionary *)header plist:(id)plist;

/*!
 @method    emulateHTTPBinaryPlistResponseOnConnection:statusLine:header:plist:
 @abstract  Emulates a bplist response, setting the Content-Length appropriately and the Content-Type to @"application/x-apple-binary-plist"
 */
- (HTTPResponseEvent *)emulateHTTPBinaryPlistResponseOnConnection:(RTEmulatedTCPConnection *)connection statusLine:(NSString *)statusLine header:(NSDictionary *)header plist:(id)plist;

@end
