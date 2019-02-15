//
//  SpecReferenceConstants.h
//  ATSMacApp
//
//  Created by Venkatesh Sivaraman on 8/10/17.
//  Copyright © 2017 Apple Inc. All rights reserved.
//

#ifndef SpecReferenceConstants_h
#define SpecReferenceConstants_h

/*
 These are constants that can be used in rules to define where spec reference 
 labels should be looked up. They are used as predicate format strings, so whenever 
 ATS performs a spec lookup, it filters the specs by whether they pass these 
 predicates. 
 
 Since the app delegate sets the SpecificationManager's spec sort comparators to 
 sort by decreasing order of revision, supplying no revision number causes the 
 lookup to default to the most recent supported spec.
 */

// Matches the most recent AISpec.
#define CurrentAISpecFileReference @"identifier == \"aispec\""
// Matches MFi Accessory Firmware Spec R46 only.
#define CurrentMFiFirmwareSpecFileReference @"MFi Accessory Firmware Specification R46"
// Matches MFi Accessory Hardware Spec R9 only.
#define CurrentMFiHardwareSpecFileReference @"MFi Accessory Hardware Specification R9"
// Matches AirPlay 2 Audio Specification R3 only.
#define CurrentAirPlay2AudioSpecFileReference @"AirPlay 2 Audio Specification R3"
// Matches AirPlay 2 Audio Certification Test Cases R3 only.
#define CurrentAirPlay2AudioCertificationTestCasesFileReference @"AirPlay 2 Audio Certification Test Cases R3"
// Matches no specs.
#define NoSpecReferenceFile @"FALSEPREDICATE"

// Not a predicate string - simply indicates that there is no spec reference.
#define NoSpecReferenceLabel @"NoSpecReference"

#endif /* SpecReferenceConstants_h */
