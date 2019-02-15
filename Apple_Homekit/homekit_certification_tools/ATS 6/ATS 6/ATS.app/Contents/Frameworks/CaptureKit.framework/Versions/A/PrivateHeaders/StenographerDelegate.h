//
//  StenographerDelegate.h
//  ATSMacApp
//
//  Created by Marin Balde on 9/20/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

@class Rule;
@class RuleError;
@class ProductPlan;

#import <CaptureKit/CaptureConfigurationDescription.h>

@protocol StenographerDelegate
@required
- (BOOL)areClaimSpecificRulesEnabled;
- (BOOL)isClaimEnabled:(NSString *)claimName;
- (void)completeRule:(Rule *)rule;
- (void)completeRule:(Rule *)rule withError:(RuleError *)error;
- (void)scheduleSurveillanceBlock:(void (^)(void))block forRule:(Rule *)rule;
- (BOOL)isAppleDeviceActingAsUSBDevice;
- (ProductPlan *)productPlan;
- (DeviceTransportType)deviceTransportType;

/*!
 @method    isEventClassAvailable:
 @abstract  Whether or not events of this class are available
 */
- (BOOL)isEventClassAvailable:(Class)aClass;

/*!
 @method        isTrafficOptional
 @abstract      Whether or not traffic (iAP) is optional on this transport
 @discussion    This was previously known as "Hardware-Only", but that term
 is implies no traffic is expected. Really it should mean
 that traffic is optional. We'll still validate it, but certain
 rules that ensure its existance (say, after power up) won't run
 */
- (BOOL)isTrafficOptional;

@end
