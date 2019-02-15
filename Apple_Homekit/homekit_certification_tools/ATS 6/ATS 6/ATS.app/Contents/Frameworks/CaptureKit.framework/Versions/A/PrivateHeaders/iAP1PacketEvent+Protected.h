//
//  iAP1PacketEvent+Protected.h
//  ATSMacApp
//
//  Created by Zachary Church on 9/16/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/iAP1PacketEvent.h>
#import <CaptureKit/DisplayProtocols.h>

/*!
 @macro     WITH_IAP_PACKET_TRIGGER
 @abstract  Create an event classification given an iAP packet lingo and command. By default, this is essential, but passing the second arg as kEventIsNotEssential will create a non-essential event classification
 @example   This is an essential IdentificationInformation: WITH_IAP_PACKET_TRIGGER(iAPSimpleRemoteLingo, iAPSimpleRemoteLingoVoiceOverEventCommand)
 @example   This is a non-essential one: WITH_IAP_PACKET_TRIGGER(iAPSimpleRemoteLingo, iAPSimpleRemoteLingoVoiceOverEventCommand, kEventIsNotEssential)
 */
#define WITH_IAP_PACKET_TRIGGER(lingoID, commandID, ...) CONSTRUCT_WITH_EVENT_TRIGGER(iAP1PacketEvent, [iAP1PacketEvent classificationForCommandWithLingo:lingoID command:commandID], ##__VA_ARGS__, YES)

/*!
 @macro     IAP_IDENTIFY_PACKETS_CLASSIFICATIONS
 @abstract  Shortcut macro to classify for any type of identification packet
 */
#define IAP_IDENTIFY_PACKETS_CLASSIFICATIONS() WITH_IAP_PACKET_TRIGGER(iAPGeneralLingo, iAPGeneralLingoSetFIDTokenValuesCommand), WITH_IAP_PACKET_TRIGGER(iAPGeneralLingo, iAPGeneralLingoIdentifyCommand), WITH_IAP_PACKET_TRIGGER(iAPGeneralLingo, iAPGeneralLingoIdentifyDeviceLingoesCommand)

@interface iAP1PacketEvent () <iAPPacketDisplay>

@end
