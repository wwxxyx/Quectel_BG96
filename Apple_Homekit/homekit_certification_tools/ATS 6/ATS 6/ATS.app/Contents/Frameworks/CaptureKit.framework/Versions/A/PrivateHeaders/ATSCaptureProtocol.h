//
//  CaptureProtocol.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 4/30/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/EventChanneling.h>

@class CaptureTransport;
@class TrafficViewController;

@interface ATSCaptureProtocol : NSObject


/*!
 Convenience method to create a protocol for input transport.

 @param transport Capture transport underlying the protocol.
 @param parentCaptureProtocol Capture protocol of which ever protocol that's creating this protocol.
        Can be nil, in case this protocol is bottom of the stack and is being created by the transport.
        Should be valid in case this protocol is carried over another protocol.
 */
+ (instancetype)protocolWithTransport:(CaptureTransport *)transport andParentCaptureProtocol:(ATSCaptureProtocol *)parentCaptureProtocol;

/*!
 Initialize for a particular transport.
 
 @param transport Capture transport underlying the protocol.
 @param parentCaptureProtocol Capture protocol of which ever protocol that's creating this protocol.
        Can be nil, in case this protocol is bottom of the stack and is being created by the transport.
        Should be valid in case this protocol is carried over the parent protocol.
 */
- (instancetype)initWithTransport:(CaptureTransport *)transport andParentCaptureProtocol:(ATSCaptureProtocol *)parentCaptureProtocol;

/*!
 Capture transport underlying the protocol.
 */
@property (nonatomic, readonly, strong) CaptureTransport *transport;

/*!
 Reference to Capture protocol this protocol is carried over. is nil, in case this protocol is bottom of the stack and is being created by the transport.
 Is valid in case this protocol is carried over another protocol.
 */
@property (nonatomic, readonly, strong) ATSCaptureProtocol *parentCaptureProtocol;

/*!
 Array of traffic view controllers associated with this transport.
 */
@property (nonatomic, readonly, strong) NSArray *trafficViewControllers;

/*!
 Return an array of available events (for RuleEngine) on this transport in the current configuration
 */
@property (nonatomic, readonly, strong) NSArray * availableEvents;

/*!
 Capture protocol's display name. Subclasses needs to implement the property and provide a display name.
 */
@property (nonatomic, readonly, strong) NSString *captureProtocolName;

/*!
 Capture protocol's parent display name. Its the name of the parent capture protocol. If parent capture protocol is nil, in case of the protocol at the bottom of the stack, transport name is returned. Example: for HID over iAP2 over UART, its parent can be iAP2 and for iAP2, its parent is the transport - UART.
 */
@property (nonatomic, readonly, strong) NSString *parentProtocolName;

/*!
 Capture protocol's display name with it's parent protocol's display name; Used as title of the protocol in ATS's category view. 
 
 @note A subclass CaptureProtocol can override it in order to customize its name.
 */
@property (nonatomic, readonly, strong) NSString *captureProtocolCategoryName;

/*!
 Add a traffic view controller associated with this transport.

 @param tvc Traffic view controller to added to the list.
 */
- (void)addTrafficViewController:(TrafficViewController *)tvc;

/*!
 Shut down anything and prepare object to be released
 */
- (void)close;

@end
