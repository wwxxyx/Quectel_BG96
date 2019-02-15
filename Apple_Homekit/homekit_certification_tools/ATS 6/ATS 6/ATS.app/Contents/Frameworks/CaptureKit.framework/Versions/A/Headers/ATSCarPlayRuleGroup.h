//
//  ATSCarPlayRuleGroup.h
//  ATSMacApp
//
//  Created by Carlos Frederico Azevedo on 3/30/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureRuleGroup.h>

/** Minimum vertical resolution required for CarPlay displays. */
extern const NSInteger ATSCarPlayScreenMinimumHeight;

/** Minimum horizontal resolution required for CarPlay displays. */
extern const NSInteger ATSCarPlayScreenMinimumWidth;

/** Minimum frame rate required for CarPlay displays. */
extern const NSInteger ATSCarPlayScreenMinimumFPS;

/** A container for CarPlay traffic validation rules. */
@interface ATSCarPlayRuleGroup : CaptureRuleGroup

- (CaptureRuleResult *)runCarPlayNCMControlInterfaceUSBHostIdentificationMismatch:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayInvalidNCMNumber:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayWiredMinimumAudioFormatsNotSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayRequiredMainDefaultAudioFormatsNotSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayRequiredAlternateDefaultAudioFormatsNotSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayRequiredAlertAudioFormatsNotSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayRequiredMainMediaAudioFormatsNotSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayRequiredCommunicationsAudioFormatsNotSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayRequiredSpeechRecognitionAudioFormatsNotSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayMinimumDisplayResolutionMismatch:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayInvalidInfoResponseName:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayInvalidBonjourServiceName:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayAudioStreamSetupTooLate:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayInvalidInfoResponsePhysicalValues:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayUSBInterfacesNotSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayInvalidHIDProductIDAndVendorID:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayLimitedUIElementsNotSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayUSBCommunicationInterfacesNotSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayUSBHostModeAudioDuringCarPlayAudioStream:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayDidNotUseControlSessionV2:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayInfoResponseHIDDescriptorUsagesNotSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayProhibitedVoiceCommandUsageSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayHiFiAndLoFiTouchFlagsSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayAudioSampleRateOutsideAllowedRange:(CaptureRule *)rule;
//- (CaptureRuleResult *)runCarPlayInvalidFeaturesFlagValue:(CaptureRule *)rule; // This rule is disabled as per radar: rdar://problem/23212211
- (CaptureRuleResult *)runCarPlayPrivateAudioModeFlagsSet:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayMaxInfoResponsePhysicalValuesExceeded:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayNoResourcesInitialModesSet:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayResourceConstraintsForTransferPriority:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayInfoResponseWithoutBluetoothIDs:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayInfoResponseBluetoothIDsIncorrectFormat:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayUSBBeginSessionTimeExceeded:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayUSBTransportInvalidIPVersion:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayWiFiTransportInvalidIPVersion:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayChangeModeBorrowTransportTypeWithTakeConstraint:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayBonjourAirPlayLocalQuestionBeforeAnswer:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayWiFiTransportMissingExtendedFeaturesParameter:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayWirelessGNSSLocationInformationMissing:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayWiredRequiredComponentsNotSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayRequiredUSBHostTransportComponentsNotSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayWirelessRequiredComponentsNotSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayRequiredUsagesForMultiAxisControllerNotSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayRequiredHIDUsagesForSingleTouchTouchScreenNotSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayRequiredUsagesForPrimaryInputDeviceTouchpadNotSeen:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayTouchpadButtonsInInvalidDescriptor:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayInfoResponseStatusFlagMustBe4:(CaptureRule *)rule;
- (CaptureRuleResult *)runCarPlayDeprecatedMacAddress:(CaptureRule *)rule;

@end
