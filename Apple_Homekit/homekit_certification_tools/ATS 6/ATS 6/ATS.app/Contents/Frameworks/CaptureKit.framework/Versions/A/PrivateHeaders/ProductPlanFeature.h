//
//  ProductPlanFeature.h
//  ATSMacApp
//
//  Created by Kip Nicol on 8/10/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

/*!
 @header     ProductPlanFeature
 @abstract   This header describes the interface for the class that represents 
             iAP1 or iAP2 features the user can claim support for in the CA. 
             The class can also be used to represent meta-features which are
             presented in the feature selection UI in order to group sets of 
             related iAP1/iAP2 featuers. They aren't real features, so they 
             don't end up in the product plan.
 */

#import <Foundation/Foundation.h>
#import <CaptureKit/ProductPlanType.h>

/*!
 @class      ProductPlanFeature
 @abstract   Encapsulates information about the different iAP1 and iAP2 features 
             presented in the CA. Meta-features don't end up in the product
             plan, but are used to group real features in the CA.
 */
@interface ProductPlanFeature : ProductPlanType

/*!
 @property   type
 @abstract   The FeatureType enum that identifies the feature to the ATS server, 
             among other things.
 */
@property (nonatomic, readonly, assign) FeatureType type;

/*!
 @property   claimName
 @abstract   The name string of the claim associated with the feature type.
 */
@property (nonatomic, readonly) NSString *claimName;

/*!
 @property   metaFeature
 @abstract   A flag indicating whether or not the feature should be treated as a 
             meta-feature.
 @discussion Meta features don't end up in the product plan, but are useful for 
             grouping related product plan features. For example, the iAP2 
             feature selection view has a "Digital Audio" feature which, when 
             selected, enables two real product plan features, USB Host Mode 
             Audio and USB Device Mode Audio.
             
             The default value is NO.
 */
@property (nonatomic, getter=isMetaFeature) BOOL metaFeature;

/*!
 @method     +nameForFeatureType:
 @abstract   Returns a string containing the official name for a feature type
 @param      type
 The feature type enum associated with the feature.
 */
+ (NSString *)nameForFeatureType:(FeatureType)type;

/*!
 @method     featureWithFeatureType:
 @abstract   Returns a new, autoreleased product plan feature initialized with 
             the given feature type.
 @param      type
             The feature type enum associated with the feature.
 */
+ (id)featureWithFeatureType:(FeatureType)type;

/*!
 @method     featureWithFeatureType:metaFeature:
 @abstract   Returns a new, autoreleased product plan meta-feature initialized 
             with the given feature type.
 @param      type
             The feature type enum associated with the feature.
 @param      metaFeature
             A flag indicating whether or not the feature should be treated as a 
             meta-feature. Meta-features are displayed in the UI but do not end 
             up in the product plan.
 */
+ (id)featureWithFeatureType:(FeatureType)type metaFeature:(BOOL)metaFeature;

/*!
 @method     initWithFeatureType:
 @abstract   Returns a new product plan feature initialized with the given 
             feature type.
 @param      type
             The feature type enum associated with the feature.
 */
- (id)initWithFeatureType:(FeatureType)type;

/*!
 @method     featureWithFeatureType:metaFeature:
 @abstract   Returns a new, autoreleased product plan meta-feature initialized
             with the given feature type.
 @param      type
             The feature type enum associated with the feature.
 @param      metaFeature
             A flag indicating whether or not the feature should be treated as a
             meta-feature. Meta-features are displayed in the UI but do not end
             up in the product plan.
 @discussion This is the designated initializer.
 */
- (id)initWithFeatureType:(FeatureType)type metaFeature:(BOOL)metaFeature;

@end
