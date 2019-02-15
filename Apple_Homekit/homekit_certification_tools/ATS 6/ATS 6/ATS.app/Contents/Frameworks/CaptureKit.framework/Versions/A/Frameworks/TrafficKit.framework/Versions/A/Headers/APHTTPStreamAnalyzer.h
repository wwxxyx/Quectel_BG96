//
//  APHTTPStreamAnalyzer.h
//  TrafficKit
//
//  Created by Mark Hamlin on 5/28/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <TrafficKit/TrafficKit.h>

@class APHTTPStreamAnalyzer;
@class APHTTPRequest;
@class APHTTPResponse;

@protocol APHTTPStreamAnalyzerDelegate
@required

/*!
 @method    httpStreamAnalyzer:parsedContentForData:type:errorDescription:httpObject:
 @abstract  Returns a parsed representation of the passed data given a type
 @return    nil if it cannot be converted to a higher level representation
 */
- (id)httpStreamAnalyzer:(APHTTPStreamAnalyzer *)analyzer parsedContentForData:(NSData *)data type:(NSString *)type errorDescription:(NSString **)errorDescription httpObject:(id)httpObject;

/*!
 @method    httpStreamAnalyzer:processHTTPRequestReturningContext:
 @abstract  Opportunity for delegate to process an HTTP request
 @return    Delegate may return a context object. This will be passed to the next response callback
 */
- (id)httpStreamAnalyzer:(APHTTPStreamAnalyzer *)analyzer processHTTPRequestReturningContext:(APHTTPRequest *)request;

/*!
 @method    httpStreamAnalyzer:processHTTPResponse:context:
 @abstract  Process an HTTP response, with the context (if any) provided for the last request
 */
- (void)httpStreamAnalyzer:(APHTTPStreamAnalyzer *)analyzer processHTTPResponse:(APHTTPResponse *)response context:(id)context;

/*!
 @method    httpStreamAnalyzer:detectedInvalidContentForFlow:
 @abstract  Invalid HTTP or non-HTTP content was detected on the given TCP stream
 */
- (void)httpStreamAnalyzer:(APHTTPStreamAnalyzer *)analyzer detectedInvalidContentForFlow:(APTCPFlow *)flow;

/*!
 @method    httpStreamAnalyzer:processIncompleteHTTPMessageData:
 @abstract  Process an incomplete HTTP message. The message was cut off during transmission or not fully received.
 */
- (void)httpStreamAnalyzer:(APHTTPStreamAnalyzer *)analyzer processIncompleteHTTPMessageData:(NSData *)messageData;

@end

@interface APHTTPStreamAnalyzer : NSObject

/**
 TCP Flow for this analyzer
 */
@property (nonatomic, readonly) APTCPFlow *flow;

/*!
 Initialize to follow the given TCP flow
 
 @param flow TCP flow that will contain HTTP payload
 @param delegate Delegate to receive the HTTP requests and responses
 @param clientIsEndpoint0 Client is the one that makes a request.
 */
- (instancetype)initWithTCPFlow:(APTCPFlow *)flow delegate:(id<APHTTPStreamAnalyzerDelegate>)delegate clientIsEndpoint0:(BOOL)clientIsEndpoint0;

/**
 Method to process decrypted TCP stream data.
 
 @param tcpData TCP stream data seen over the TCP connection responsible for sending HTTP payloads for controller control or accessory control.
 @param senderIsZero Whether or not the sender is at endpoint zero.
 */
- (void)processTCPStreamData:(NSData *)tcpData senderIsZero:(BOOL)senderIsZero;

@end
