//
//  RuleTests+IDBusPacket.h
//  ATSMacApp
//
//  Created by anchor on 3/4/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/RuleTests.h>
#import <CaptureKit/NSData+Extensions.h>
#import <CaptureKit/ProductPlanLightningModuleFunctionalVariant.h>

@interface RuleTests (IDBusPacket)

/*!
 @method    allLightningModuleFunctionalVariants
 @abstract  Returns an array of all available lightning module functional variant enums (LightningModuleFunctionalVariant)
 */
- (NSArray *)allLightningModuleFunctionalVariants;

/*!
 @method    allLightningModuleFunctionalVariantsExternal
 @abstract  Returns an array of all available lightning module functional variant enums (LightningModuleFunctionalVariant)
            that are externally available for 3rd party use.
 */
- (NSArray *)allLightningModuleFunctionalVariantsExternal;

/*!
 @method    variantDataForLightningModuleFunctionalVariant:
 @abstract  Returns the variant data associated with the given variant.
 */
- (NSData *)variantDataForLightningModuleFunctionalVariant:(LightningModuleFunctionalVariant)lightningModuleFunctionVariant;

/*!
 @method    isLightningModuleFunctionVariantExternal:
 @abstract  Returns whether the given variant is externally availble for 3rd party use.
 */
- (BOOL)isLightningModuleFunctionVariantExternal:(LightningModuleFunctionalVariant)lightningModuleFunctionVariant;

@end
