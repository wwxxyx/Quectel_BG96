//
//  ATSCarPlayResetEvent.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 7/6/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/TCPConnectionEvent.h>

@class APTCPFlow;

typedef NS_ENUM(NSUInteger, CarPlayTCPConnectionType) {
    /**
     RTSP Controller control flow; Direction, device to accessory; iOS control
     */
    CarPlayTCPConnectionTypeController,
    /**
      RTSP Accessory control flow; Direction, accessory to device; Accessory control
     */
    CarPlayTCPConnectionTypeAccessory,
    /**
     RTP Screen flow; Direction, device to accessory; User interface
     */
    CarPlayTCPConnectionTypeScreen,
    /**
     CarPlay Control
     */
    CarPlayTCPConnectionTypeCarPlayControl,
};

/**
 CarPlay controller stream closed event is created when Controller or Accessory control flow (TCP flow end) sees a fin flag.
 */
@interface CarPlayTCPConnectionEvent : TCPConnectionEvent

/**
 Stream type for which TCP flow ended.
 */
@property (nonatomic, readonly) CarPlayTCPConnectionType tcpStreamType;

/**
 Whether or not endpoint of the TCP flow is iOS device. If the value is NO, then it's a CarPlay accessory on endpoint 0.
 */
@property (nonatomic, readonly) BOOL isAppleDeviceEndpoint0;

/**
 IP address of the iOS device.
 */
@property (nonatomic, readonly) NSString *iOSDeviceIPAddress;

/**
 IP address of the CarPlay accessory.
 */
@property (nonatomic, readonly) NSString *carPlayAccessoryIPAddress;

/**
 Create an instance of CarPlay controller stream closed event.
 
 @param timestamp timestamp for this event.
 @param isSuitableForRuleEngine Whether or not the event created using the data is suitable for rule engine.
 @param tcpFlow The CarPlay TCP flow associated with the event.
 @param tcpStreamType Stream type for which TCP flow ended.
 @param tcpStreamStatus Wheather this Carplay TCP stream event is newly opened or just closed.
 @param isAppleDeviceEndpoint0 Whether or not endpoint of the TCP flow is iOS device. If the value is NO, then it's a CarPlay accessory on endpoint 0.
*/
+ (instancetype)eventWithTimestamp:(UInt64)timestamp
             suitableForRuleEngine:(BOOL)isSuitableForRuleEngine
                           tcpFlow:(APTCPFlow *)tcpFlow
                     tcpStreamType:(CarPlayTCPConnectionType)tcpStreamType
                  connectionStatus:(TCPConnectionStatus)connectionStatus
            isAppleDeviceEndpoint0:(BOOL)isAppleDeviceEndpoint0;

@end
