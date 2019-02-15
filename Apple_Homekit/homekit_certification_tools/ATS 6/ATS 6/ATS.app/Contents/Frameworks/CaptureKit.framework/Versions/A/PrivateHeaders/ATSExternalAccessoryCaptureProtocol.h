//
//  ATSExternalAccessoryCaptureProtocol.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 4/28/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSCaptureProtocol.h>
#import <BusSniffer/BSiAPTypes.h>
#import <BusSniffer/BSErrorDomain.h>

@interface ATSExternalAccessoryCaptureProtocol : ATSCaptureProtocol

/**
 Initialize EA Capture protocol for a particular transport.
 
 @param transport Capture Transport for the protocol.
 @param parentCaptureProtocol Reference to parent capture protocol.
 */
+ (instancetype)protocolWithTransport:(CaptureTransport *)transport andParentCaptureProtocol:(ATSCaptureProtocol *)parentCaptureProtocol;

/**
 External accessory capture protocol's event channel. Used to distinguish between events to two EA TVCs over same transport.
 */
@property (nonatomic, strong, readonly) id<EventChanneling> eventChannel;

@property (nonatomic, readonly, strong) NSString *captureProtocolName;

/**
 Create an EA packet. Use that to create an EA event. The event is then delivered to the transport, which then broadcasts it to it's listeners (Currently, ATSExternalAccessoryTrafficViewController).
 
 @param data Data to process. Used to create the EA packet.
 @param identifier session identifier for the EA session if there is any. There isn't a session identifier in case
 @param timestamp TimeStamp associated with the packet.
 @param packetErrors Errors already associated with this packet, determined by the lower protocol layers like iAP2 or IDBus
 @param isSuitableForRuleEngine Whether or not the event created using the data is suitable for rule engine.
 */
- (void)processData:(NSData *)data
     withIdentifier:(NSNumber *)identifier
         fromSource:(BSiAPSource)source
        atTimestamp:(UInt64)timestamp
       packetErrors:(NSArray <NSError *>*)errors
suitableForRuleEngine:(BOOL)isSuitableForRuleEngine;

@end
