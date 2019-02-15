//
//  SelfCertClaimSelectConstants.h
//  BBSniff
//
//  Created by Marin Balde on 9/16/11.
//  Copyright 2011 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#define CHILD_CLAIM_NAME(parent, child) [NSString stringWithFormat:@"%@\\%@", parent, child]

extern NSString *const ClaimNameUsesiAP;
extern NSString *const ClaimNameCommunicationWithiOSApps;
extern NSString *const ClaimNameGymEquipment;
extern NSString *const ClaimNameHighPower;
extern NSString *const ClaimNameLineOutAccessory;
extern NSString *const ClaimNameLocation;
extern NSString *const ClaimNameRFTuner;
extern NSString *const ClaimNameRemoteUI;
extern NSString *const ClaimNameSimpleRemoteAccessory;
extern NSString *const ClaimNameStorageLingoAccessory;
extern NSString *const ClaimNameUSBHostModeLingo; // NOTE: this claim refers to the LINGO and *not* the TRANSPORT see rdar://problem/10429748
extern NSString *const ClaimNameiAPOverUSB;
extern NSString *const ClaimNameVideoOutAccessory;
extern NSString *const ClaimNameWiFiNetworkLoginSharing;
extern NSString *const ClaimNameAssistiveTouch;
extern NSString *const ClaimNameiPodOut;

extern NSString *const ClaimNameAccessoryIdentifyResistor;
// SubClaimName* should only be used to construct the child claim names below
// Do not use SubClaimName* in Rule definitions
extern NSString *const SubClaimNameAccessoryIdentifyResistor191k;
extern NSString *const SubClaimNameAccessoryIdentifyResistor255k;
extern NSString *const SubClaimNameAccessoryIdentifyResistor28k;
extern NSString *const SubClaimNameAccessoryIdentifyResistor549k;
#define ClaimNameAccessoryIdentifyResistor191k CHILD_CLAIM_NAME(ClaimNameAccessoryIdentifyResistor, SubClaimNameAccessoryIdentifyResistor191k)
#define ClaimNameAccessoryIdentifyResistor255k CHILD_CLAIM_NAME(ClaimNameAccessoryIdentifyResistor, SubClaimNameAccessoryIdentifyResistor255k)
#define ClaimNameAccessoryIdentifyResistor28k CHILD_CLAIM_NAME(ClaimNameAccessoryIdentifyResistor, SubClaimNameAccessoryIdentifyResistor28k)
#define ClaimNameAccessoryIdentifyResistor549k CHILD_CLAIM_NAME(ClaimNameAccessoryIdentifyResistor, SubClaimNameAccessoryIdentifyResistor549k)

extern NSString *const ClaimNameUSBVbusPowerSource;
// SubClaimName* should only be used to construct the child claim names below
// Do not use SubClaimName* in Rule definitions
extern NSString *const SubClaimNameUSBVbusPowerSource1A;
extern NSString *const SubClaimNameUSBVbusPowerSource2Point1A;
extern NSString *const SubClaimNameUSBVbusPowerSource2Point4A;
extern NSString *const SubClaimNameUSBVbusPowerSource500mA;
#define ClaimNameUSBVbusPowerSource1A CHILD_CLAIM_NAME(ClaimNameUSBVbusPowerSource, SubClaimNameUSBVbusPowerSource1A)
#define ClaimNameUSBVbusPowerSource2Point1A CHILD_CLAIM_NAME(ClaimNameUSBVbusPowerSource, SubClaimNameUSBVbusPowerSource2Point1A)
#define ClaimNameUSBVbusPowerSource2Point4A CHILD_CLAIM_NAME(ClaimNameUSBVbusPowerSource, SubClaimNameUSBVbusPowerSource2Point4A)
#define ClaimNameUSBVbusPowerSource500mA CHILD_CLAIM_NAME(ClaimNameUSBVbusPowerSource, SubClaimNameUSBVbusPowerSource500mA)

// Legacy claim names (only used for compatibility with old SelfCertDataModel's)
extern NSString *const LegacyClaimNameAppleDeviceIsUSBHostTransport; // Do not use in rules. NOTE: this claim refers to the TRANSPORT and *not* the LINGO see rdar://problem/10429748
extern NSString *const LegacyClaimNameAccessibility;
extern NSString *const LegacyClaimNameCommunicationWithiPhoneOSApplications;
extern NSString *const LegacySubClaimNameAccessoryIdentifyResistor1M;
extern NSString *const LegacySubClaimNameAccessoryIdentifyResistor360k;
#define LegacyClaimNameAccessoryIdentifyResistor1M CHILD_CLAIM_NAME(ClaimNameAccessoryIdentifyResistor, LegacySubClaimNameAccessoryIdentifyResistor1M)
#define LegacyClaimNameAccessoryIdentifyResistor360k CHILD_CLAIM_NAME(ClaimNameAccessoryIdentifyResistor, LegacySubClaimNameAccessoryIdentifyResistor360k)

extern NSString *const ClaimNameiAP2AppLaunch;
extern NSString *const ClaimNameiAP2AssistiveTouch;
extern NSString *const ClaimNameiAP2BluetoothConnection;
extern NSString *const ClaimNameiAP2DeviceAuthentication;
extern NSString *const ClaimNameiAP2DigitalAudioHostMode;
extern NSString *const ClaimNameiAP2DigitalAudioDeviceMode;
extern NSString *const ClaimNameiAP2EASessioniAP2;
extern NSString *const ClaimNameiAP2EANativeTransport;
extern NSString *const ClaimNameiAP2HIDSessioniAP2;
extern NSString *const ClaimNameiAP2HIDNativeTransport;
extern NSString *const ClaimNameiAP2Location;
extern NSString *const ClaimNameiAP2MediaLibraryInformation;
extern NSString *const ClaimNameiAP2MediaLibraryUpdates;
extern NSString *const ClaimNameiAP2MediaLibraryPlayback;
extern NSString *const ClaimNameiAP2NowPlaying;
extern NSString *const ClaimNameiAP2PowerDevicePowered;
extern NSString *const ClaimNameiAP2VoiceOver;
extern NSString *const ClaimNameiAP2WiFi;
extern NSString *const ClaimNameiAP2HIDSessioniAP2WithHIDFunctionKeyboard;
extern NSString *const ClaimNameiAP2HIDSessioniAP2WithHIDFunctionMediaPlaybackRemote;
extern NSString *const ClaimNameiAP2HIDSessioniAP2WithHIDFunctionAssistiveTouchPointer;
extern NSString *const ClaimNameiAP2HIDNativeTransportWithHIDFunctionKeyboard;
extern NSString *const ClaimNameiAP2HIDNativeTransportWithHIDFunctionMediaPlaybackRemote;
extern NSString *const ClaimNameiAP2HIDNativeTransportWithHIDFunctionAssistiveTouchPointer;
extern NSString *const ClaimNameiAP2DeviceNotifications;
extern NSString *const ClaimNameiAP2HIDSessioniAP2WithHIDFunctionGameController;
extern NSString *const ClaimNameiAP2HIDSessioniAP2WithHIDFunctionAssistiveSwitchControl;
extern NSString *const ClaimNameiAP2HIDNativeTransportWithHIDFunctionGameController;
extern NSString *const ClaimNameiAP2HIDNativeTransportWithHIDFunctionAssistiveSwitchControl;
extern NSString *const ClaimNameiAP2Telephony;
extern NSString *const ClaimNameiAP2VehicleStatus;
extern NSString *const ClaimNameiAP2HIDSessioniAP2WithHIDFunctionHeadphone;
extern NSString *const ClaimNameiAP2HIDNativeTransportWithHIDFunctionHeadphone;
extern NSString *const ClaimNameiAP2MultipleEASessions;
