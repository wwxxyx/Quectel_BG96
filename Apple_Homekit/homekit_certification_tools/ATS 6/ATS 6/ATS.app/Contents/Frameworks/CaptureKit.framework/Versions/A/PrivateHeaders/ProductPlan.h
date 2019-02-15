//
//  ProductPlan.h
//  ATSMacApp
//
//  Created by Kip Nicol on 7/21/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/SelfCertTestConfiguration.h>
#import <CaptureKit/ProductPlanAppleDeviceTestParameter.h>
#import <CaptureKit/ProductPlanAppleDevice.h>
#import <CaptureKit/ProductPlanConnector.h>
#import <CaptureKit/ProductPlanFeature.h>
#import <CaptureKit/ProductPlaniAPVersion.h>
#import <CaptureKit/ProductPlanLightningModuleFunctionalVariant.h>
#import <CaptureKit/ProductPlanSelfCertTest.h>
#import <CaptureKit/ProductPlanUSBVbusPowerSource.h>
#import <CaptureKit/ProductPlanResistor.h>
#import <CaptureKit/ProductPlanTypes.h>

@class SKProductPlan;
@class ProductPlanAppleDevice;

@interface ProductPlan : NSObject <NSCoding>

@property (nonatomic, readonly) NSString *ppid;
@property (nonatomic, readonly) NSString *sku;
@property (nonatomic, readonly) NSString *name;
@property (nonatomic, readonly) NSArray<ProductPlanAppleDevice *> *supportedDevices;
@property (nonatomic, readonly) NSArray<ProductPlanFeature *> *selectediAPFeatures;
@property (nonatomic, readonly) NSArray *transports;
@property (nonatomic, readonly) BOOL supportsCarPlay;
@property (nonatomic, readonly) NSString *state;

/*!
 @property  lightningModuleFunctionalVariant
 @abstract  The Product Plan's corresponding Lightning module functional variants.
 */
@property (nonatomic, readonly, strong) NSArray *lightningModuleFunctionalVariants;
@property (nonatomic, strong) SelfCertTestConfiguration *testParameters;

@property (nonatomic, readonly) BOOL isTestModeAccessory;

/*! Filled with GetProductPlanDetails. If the user selected to use 30-Pin connectors*/
@property (nonatomic, readonly) BOOL is30PinConnector;

/*! Filled with GetProductPlanDetails. If the user selected to use Lightning connectors*/
@property (nonatomic, readonly) BOOL isLightningConnector;
@property (nonatomic) iAPVersionType supportediAPProtocols;

- (id)initWithProductID:(NSString *)productID
                        accessorySKU:(NSString *)accessorySKU
                       accessoryName:(NSString *)accessoryName
    lightningModuleFunctionalVariants:(NSArray *)lightningModuleFunctionalVariants
                      testParameters:(SelfCertTestConfiguration *)testParameters;

+ (instancetype)productPlanWithSKProductPlan:(SKProductPlan *)productPlan;
+ (instancetype)productPlanWithDictionary:(NSDictionary *)dict;

@end
