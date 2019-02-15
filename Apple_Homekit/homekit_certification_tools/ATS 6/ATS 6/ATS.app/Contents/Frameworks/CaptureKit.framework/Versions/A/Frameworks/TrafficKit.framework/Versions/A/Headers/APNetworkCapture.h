//
//  APNetworkCapture.h
//  APTS
//
//  Created by Mark Hamlin on 6/21/12.
//  Copyright (c) 2012 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>

@class APCaptureDevice;
@class APPacket;
@class APUDPFlowTable;
@class APTCPFlowTable;
@class APTime;
@class APNetworkCapture;

@protocol APNetworkCaptureFilteredPacketDelegate <NSObject>
@required

/*!
 Delegate method to notify when a packet is seen.
 
 @param networkCapture Reference to self
 @param packet A new packet
 */
- (void)networkCapture:(APNetworkCapture *)networkCapture didReceiveFilteredPacket:(APPacket *)packet;

@end

/*!
 @class         APNetworkCapture
 @abstract      Manages a packet capture
 @discussion    This class will make available packet flows as they are detected
 */
@interface APNetworkCapture : NSObject

/*!
 @property device
 @abstract The capture device being used
 */
@property (readonly, nonatomic, strong) APCaptureDevice *device;

/*!
 Delegate to receive filtered (relevant) APPackets
 */
@property (weak) id<APNetworkCaptureFilteredPacketDelegate> filteredPacketDelegate;

/*!
 Sets up a network capture on the given device
 @param device Device to capture on
 @param timeManager time manager
 @param udpFlowTable UDP Flow Table that receieves UDP packets
 @param tcpFlowTable TCP Flow Table that receieves TCP packets
 @return The network capture
 */
+ (APNetworkCapture *)captureWithDevice:(APCaptureDevice *)device
                            timeManager:(APTime *)timeManager
                           udpFlowTable:(APUDPFlowTable *)udpFlowTable
                           tcpFlowTable:(APTCPFlowTable *)tcpFlowTable;


/*!
 @method    startWithError:
 @abstract  Starts the capture
 @param     error
            Pointer to an NSString which will be assigned if the capture failed to start
 @return    Whether or not the capture began
 */
- (BOOL)startWithError:(NSString **)error;

/*!
 @method        stop
 @abstract      Stops the capture
 @discussion    If the capture is running, it must be stopped before being discarded
 */
- (void)stop;

/*!
 @method    isRunning
 @abstract  Whether or not the capture is currently in progress
 @return    Capture status
 */
- (BOOL)isRunning;

/*!
 @method    snaplen
 @abstract  Maximum packet capture length
 */
+ (unsigned int)snaplen;

/*!
 @method    timeManager
 @abstract  Time manager instance for this capture
 */
- (APTime *)timeManager;

#pragma mark -
#pragma mark Null Device Captures

/*!
 Returns a capture without any device, where it is the app's responsibility to call -processRawPacketData:timestamp:
 
 @param timeManager time manager
 @param udpFlowTable UDP Flow Table that receieves UDP packets
 @param tcpFlowTable TCP Flow Table that receieves TCP packets
 @return The network capture
 */
+ (APNetworkCapture *)captureNullDeviceWithTimeManager:(APTime *)timeManager
                                          udpFlowTable:(APUDPFlowTable *)udpFlowTable
                                          tcpFlowTable:(APTCPFlowTable *)tcpFlowTable;

/*!
 @method    processRawPacketData:timestamp:
 @abstract  Process packet data starting with the link layer
 */
- (BOOL)processRawPacketData:(NSData *)data timestamp:(double)timestamp;

#pragma mark -
#pragma mark External Decoders

/*!
 @method    transformCurrentPacket:
 @abstract  Transforms the current packet into a higher-level parsed representation, for things like RTP where the decoding is context sensitive
 */
- (void)transformCurrentPacket:(APPacket *)packet;

@end
