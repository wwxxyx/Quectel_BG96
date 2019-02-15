//
//  SelfCertTestConfiguration.h
//  ATSMacApp
//
//  Created by Kip Nicol on 7/21/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/ProductPlanTypes.h>

@class ProductPlanConnector;
@class ProductPlaniAPVersion;
@class ProductPlanUSBVbusPowerSource;
@class ProductPlanResistor;
@class ProductPlanTransport;

@interface SelfCertTestConfiguration : NSObject <NSCoding>

@property (nonatomic, readonly, retain) ProductPlanConnector *connector;
@property (nonatomic, readonly, retain) ProductPlaniAPVersion *iapVersion;
@property (nonatomic, readonly, retain) ProductPlanUSBVbusPowerSource *charger;
@property (nonatomic, readonly, retain) ProductPlanResistor *resistor;
@property (nonatomic, readonly, retain) ProductPlanTransport *transport;
@property (nonatomic, readonly, assign) uint32_t rfCertificationBitmask;
@property (nonatomic, readonly, retain) NSSet *features;
@property (nonatomic, readonly, retain) NSArray *appleDeviceTestParameters;
@property (nonatomic, retain) NSString *connectorNameWithIndex;
@property (nonatomic, assign) NSUInteger connectorIndex;

@property (nonatomic, readonly, copy) NSString *validationFailureMessage;
@property (nonatomic, readonly) BOOL isValid;

@property (nonatomic, readonly, retain) NSSet *claimNames;

- (id)initWithConnector:(ProductPlanConnector *)connector
                   iapVersion:(ProductPlaniAPVersion *)iapVersion
                      charger:(ProductPlanUSBVbusPowerSource *)charger
                     resistor:(ProductPlanResistor *)resistor
                    transport:(ProductPlanTransport *)transport
       rfCertificationBitmask:(uint32_t)rfCertificationBitmask
                     features:(NSSet *)features
    appleDeviceTestParameters:(NSArray *)appleDeviceTestParameters;

- (id)initWithValidationFailureMessage:(NSString *)failureMessage;
- (id)initWithDictionary:(NSDictionary *)dict;

@end
