//
//  CaptureKit.h
//  CaptureKit
//
//  Created by Zachary Church on 7/25/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//
// https://gitlab.sd.apple.com/ats/CaptureKitDocumentation

@import Cocoa;

#pragma mark - Capture Document

#import <CaptureKit/CaptureKitManager.h>
#import <CaptureKit/CaptureConfigurationManager.h>
#import <CaptureKit/CaptureConfigurationManagerDelegate.h>
#import <CaptureKit/CaptureDocument.h>
#import <CaptureKit/CaptureDocument+OutlineViewDelegate.h>
#import <CaptureKit/CaptureConfiguration.h>
#import <CaptureKit/CaptureConfigurationTypes.h>
#import <CaptureKit/DeviceCaptureConfigurationTypes.h>
#import <CaptureKit/CaptureTransport.h>

#pragma mark - Traffic View Controllers

#import <CaptureKit/TrafficViewController.h>

#pragma mark - Capture Devices

#import <CaptureKit/CaptureDevice.h>
#import <CaptureKit/BeagleUSBCaptureDeviceManager.h>
#import <CaptureKit/PhysicalBeagleUSBCaptureDevice.h>
#import <CaptureKit/WiFiCaptureDeviceManager.h>
#import <CaptureKit/BeagleUSBCaptureDevice.h>

#pragma mark - Events

#import <CaptureKit/CaptureEvent.h>
#import <CaptureKit/EventBroker.h>
#import <CaptureKit/EventIndirectlySubscribable.h>
#import <CaptureKit/EventChanneling.h>
#import <CaptureKit/MessageEvent.h>

#pragma mark - AirPlay 2 Events

#import <CaptureKit/PTPMessageEvent.h>

#pragma mark - CarPlay Events

#import <CaptureKit/CarPlayMessages.h>
#import <CaptureKit/CarPlaySessionEvent.h>
#import <CaptureKit/CarPlayControlRequestEvent.h>
#import <CaptureKit/CarPlayControlResponseEvent.h>

#pragma mark - iAP2 Events

#import <CaptureKit/iAP2Event.h>
#import <CaptureKit/iAP2LinkPacketEvent.h>
#import <CaptureKit/iAP2MessageEvent.h>
#import <CaptureKit/iAP2LinkResetEvent.h>
#import <CaptureKit/iAP2LinkSynchronizationEvent.h>
#import <CaptureKit/iAP2MissingLinkPacketEvent.h>
#import <CaptureKit/iAP2Constants.h>

#pragma mark - iAP1 Events

#import <CaptureKit/iAP1PacketEvent.h>

#pragma mark - USB Events

#import <CaptureKit/USBEvent.h>
#import <CaptureKit/USBSpeedChangeEvent.h>
#import <CaptureKit/USBResetEvent.h>
#import <CaptureKit/USBTransferEvent.h>

#pragma mark - Network Events

#import <CaptureKit/HTTPRequestEvent.h>
#import <CaptureKit/HTTPResponseEvent.h>

#pragma mark - Connection Events

#import <CaptureKit/ConnectEvent.h>
#import <CaptureKit/DisconnectEvent.h>
#import <CaptureKit/AppleDeviceConnectEvent.h>
#import <CaptureKit/AppleDeviceDisconnectEvent.h>
#import <CaptureKit/AccessoryConnectEvent.h>
#import <CaptureKit/AccessoryDisconnectEvent.h>

#pragma mark - App Events

#import <CaptureKit/ATSAppEvent.h>
#import <CaptureKit/TestingLifecycleEvent.h>
#import <CaptureKit/StartContextEvent.h>
#import <CaptureKit/StopContextEvent.h>

#pragma mark - Context Events

#import <CaptureKit/ContextEvent.h>
#import <CaptureKit/StartContextEvent.h>
#import <CaptureKit/StopContextEvent.h>

#pragma mark - Error Events

#import <CaptureKit/ErrorEvent.h>
#import <CaptureKit/RuleError.h>
#import <CaptureKit/ATSCaptureError.h>
#import <CaptureKit/ParseError.h>

#pragma mark - Rules

#import <CaptureKit/CaptureRuleGroupManager.h>
#import <CaptureKit/ATSCarPlayRuleGroup.h>
#import <CaptureKit/RuleConstants_Generated.h> // iAP Rules (iAP1 and iAP2)

#pragma mark - Bundled Frameworks

#import <iAP2MessageKit/iAP2MessageKit.h>
#import <BusSniffer/BusSniffer.h>

#pragma mark - Spec References

#import <CaptureKit/SpecReferenceConstants.h>

#pragma mark - Expose Objective-C classes To Swift

#import <CaptureKit/TimestampFormatter.h>
