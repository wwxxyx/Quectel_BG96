//
//  ATSCarPlayCaptureProtocol.h
//  
//
//  Created by Zachary Church on 10/5/15.
//
//

#import <CaptureKit/ATSCaptureProtocol.h>
#import <TrafficKit/APHTTPBPlistContainer.h>
#import <CaptureKit/CarPlayMessages.h>

@class HTTPRequestEvent;
@class HTTPResponseEvent;
@class APHTTPRequest;
@class APHTTPResponse;
@class ATSCarPlayCaptureProtocol;

@protocol ATSCarPlayCaptureProtocolDelegate

- (void)carPlayProtocol:(ATSCarPlayCaptureProtocol *)protocol detectedControllerMainAudioPort:(NSUInteger)port;
- (void)carPlayProtocol:(ATSCarPlayCaptureProtocol *)protocol detectedAccessoryControlPort:(NSUInteger)port;

- (void)carPlayProtocol:(ATSCarPlayCaptureProtocol *)protocol detectedPort:(NSUInteger)port forAccessoryStream:(CPStreamID)streamType;

@end

@interface ATSCarPlayCaptureProtocol : ATSCaptureProtocol <APHTTPBPlistDataSource>

- (void)processHTTPRequestEvent:(HTTPRequestEvent *)event;
- (void)processHTTPResponseEvent:(HTTPResponseEvent *)event;

@property (nonatomic, readonly, strong) NSString *captureProtocolName;

@property (nonatomic, weak) id<ATSCarPlayCaptureProtocolDelegate> delegate;

@end
