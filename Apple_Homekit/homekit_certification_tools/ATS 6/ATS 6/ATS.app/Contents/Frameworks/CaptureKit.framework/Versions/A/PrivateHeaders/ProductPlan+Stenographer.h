//
//  ProductPlan+Stenographer.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 8/2/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/ProductPlan.h>

#import <CaptureKit/ProductPlanTypes.h>

/*!
 @class     ProductPlan (Stenographer)
 @abstract  A category on ProductPlan to parse out parameters relevant to Stenographer rule tests
 */
@interface ProductPlan (Stenographer)

/*!
 @method    appleDevices
 @abstract  All claimed Apple devices
 @return    Array of claimed Apple devices
 @see       AppleDeviceType
 */
- (NSArray *)appleDevices;

/*!
 @method    supportsiPad
 @abstract  Checks if an iPad is among the Apple devices
 */
- (BOOL)supportsiPad;

/*!
 @method    supportsiPod
 @abstract  Checks if an iPod is among the Apple devices
 */
- (BOOL)supportsiPod;

/*!
 @method    supportsiPhone
 @abstract  Checks if an iPhone is among the Apple devices
 */
- (BOOL)supportsiPhone;

/*!
 @method    features
 @abstract  Array of all feature types
 @return    Array of feature types
 @see       FeatureType
 */
- (NSArray *)features;

/*!
 @method    supportsFeature:
 @abstract  Checks if the ProductPlan claims support for the passed feature
 */
- (BOOL)supportsFeature:(FeatureType)feature;

/*!
 @method    transport
 @abstract  Determintes the iAP transport
 */
- (TransportType)transport;

/*!
 @method    claimedPowerSourceType
 @abstract  The type of power source they have claimed (Vbus claim)
 */
- (USBVbusPowerSourceType)claimedPowerSourceType;

@end
