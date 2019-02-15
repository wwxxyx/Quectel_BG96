//
//  iPodModelUtils.h
//  BBSniff
//
//  Created by Bob Burrough on 6/24/10.
//  Copyright 2010 Apple Inc.. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <CaptureKit/ProductPlanTypes.h>

// Legacy iPod model names (only used for compatibility with pre-2.2 SelfCertDataModel's)
extern NSString *const LegacyiPodModelNameiPad2;
extern NSString *const LegacyiPodModelNameiPad;
extern NSString *const LegacyiPodModelNameiPhone4S;
extern NSString *const LegacyiPodModelNameiPhone4;
extern NSString *const LegacyiPodModelNameiPhone3GS;
extern NSString *const LegacyiPodModelNameiPhone3G;
extern NSString *const LegacyiPodModelNameiPhone;
extern NSString *const LegacyiPodModelNameiPodNano6G;
extern NSString *const LegacyiPodModelNameiPodNano5G;
extern NSString *const LegacyiPodModelNameiPodNano4G;
extern NSString *const LegacyiPodModelNameiPodNano3G;
extern NSString *const LegacyiPodModelNameiPodNano2G;
extern NSString *const LegacyiPodModelNameiPodNano1G;
extern NSString *const LegacyiPodModelNameiPodTouch4G;
extern NSString *const LegacyiPodModelNameiPodTouch3G;
extern NSString *const LegacyiPodModelNameiPodTouch2G;
extern NSString *const LegacyiPodModelNameiPodTouch1G;
extern NSString *const LegacyiPodModelNameiPodClassic;
extern NSString *const LegacyiPodModelNameiPodWithVideo;
extern NSString *const LegacyiPodModelNameiPodPhoto;
extern NSString *const LegacyiPodModelNameiPod5G;
extern NSString *const LegacyiPodModelNameiPod4G;
extern NSString *const LegacyiPodModelNameiPod3G;
extern NSString *const LegacyiPodModelNameiPodMini;

//take the long iPod model (like "iPhone 3GS") and shorten it to either iPod, iPhone or iPad.
NSString *shortiPodModelName(NSString *iPodModelName);

//given a model name, decide whether this is a Darwin device
BOOL isiPodModelDarwin(NSString *iPodModelName);

//returns yes if the Certification Assistant should attempt to positively identify the iPod model
BOOL isiPodModelIdentifiable(NSString *iPodModelName);

/*!
 @function  AppleDeviceTypeFromModelString
 @abstract  Converts a model string to an AppleDeviceType
 */
AppleDeviceType AppleDeviceTypeFromModelString(NSString *modelString);

/*!
 @function  AppleDeviceTypeFromModelIDPrefix
 @abstract  Determines the AppleDeviceType from the model id prefix
 */
AppleDeviceType AppleDeviceTypeFromModelIDPrefix(UInt16 modelIDPrefix);

/*!
 @function  isiPadAppleDeviceType
 @abstract  Whether or not the provided Apple device is an iPad
 @param     appleDeviceType
            The ProductPlan Apple device type
 */
BOOL isiPadAppleDeviceType(AppleDeviceType appleDeviceType);

/*!
 @function  isiPodAppleDeviceType
 @abstract  Whether or not the provided Apple device is an iPod
 @param     appleDeviceType
            The ProductPlan Apple device type
 */
BOOL isiPodAppleDeviceType(AppleDeviceType appleDeviceType);

/*!
 @function  isiPhoneAppleDeviceType
 @abstract  Whether or not the provided Apple device is an iPhone
 @param     appleDeviceType
            The ProductPlan Apple device type
 */
BOOL isiPhoneAppleDeviceType(AppleDeviceType appleDeviceType);
