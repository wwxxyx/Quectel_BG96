//
//  ProductPlanLightningModuleFunctionalVariant.h
//  ATSMacApp
//
//  Created by Kip Nicol on 10/16/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/ProductPlanType.h>
#import <IDBusKit/IDBusKit.h>

@interface ProductPlanLightningModuleFunctionalVariant : ProductPlanType

/*!
 @property  variant
 @abstract  Returns the variant of the Lightning Module represented by this object.
 */
@property (nonatomic, readonly, assign) LightningModuleFunctionalVariant variant;

/*!
 @property  functionalVariantIDValue
 @abstract  Returns the bitmask associated with the object's @c variant.
 */
@property (nonatomic, readonly) UInt64 functionalVariantIDValue;

/*!
 @method    lightningModuleFunctionalVariant:
 @abstract  Returns an initialized object.
 @param     variant
            The @c LightningModuleFunctionalVariant variant.
 */
+ (id)lightningModuleFunctionalVariant:(LightningModuleFunctionalVariant)variant;

/*!
 @method    initWithLightningModuleFunctionalVariant
 @abstract  Initializes and returns the object.
 @param     variant
            The @c LightningModuleFunctionalVariant variant.
 */
- (id)initWithLightningModuleFunctionalVariant:(LightningModuleFunctionalVariant)variant;

/*!
 @method    lightningModuleFunctionalVariantsFromIDValue:
 @abstract  Returns an array of @c ProductPlanLightningModuleFunctionalVariant which have a
            functional variant which matches the value passed in. Due to the fact that there
            are multiple variants for each ID value, this method returns an array. Returns
            nil if the variant is not valid.
 @param     idValue
            The functional variant's ID value.
 */
+ (NSArray *)lightningModuleFunctionalVariantsFromIDValue:(IDBFunctionalVariantID)idValue;

/*!
 @method    lightningModuleConfigurationNameFromIDValue:
 @abstract  Returns the configuration name for the variant ID.
 @param     idValue
            The functional variant's ID value.
 */
+ (NSString *)lightningModuleConfigurationNameFromIDValue:(IDBFunctionalVariantID)idValue;

/*!
 @method    isLightningModuleConfigurationDeprecated:
 @abstract  Returns wether the give variant ID is deprecated.
 @param     idValue
            The functional variant's ID value.
 */
+ (BOOL)isLightningModuleConfigurationDeprecated:(IDBFunctionalVariantID)idValue;

@end
