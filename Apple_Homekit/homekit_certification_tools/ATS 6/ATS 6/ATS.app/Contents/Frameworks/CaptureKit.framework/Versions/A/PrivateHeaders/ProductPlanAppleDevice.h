//
//  ProductPlanAppleDevice.h
//  ATSMacApp
//
//  Created by Kip Nicol on 8/10/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/ProductPlanType.h>
#import <CaptureKit/ProductPlaniAPVersion.h>
#import <CaptureKit/ProductPlanUSBVbusPowerSource.h>

/*!
 @class      ProductPlanAppleDevice
 @abstract   This header file describes the interface for the
             class ProductPlanAppleDevice. This class is used to determine the
             required tests for each apple device in the product plan, based on
             the selected configuration and features.
 */
@interface ProductPlanAppleDevice : ProductPlanType {
}

/*!
 @property  AppleDeviceType
 @abstract  Returns the apple device of thie instance of the ProductPlanAppleDevice object.
 */
@property (nonatomic, readonly, assign) AppleDeviceType type;

/*!
 @property  dockConnectorType
 @abstract  Returns the dock connector that's supported on the given device.
 */
@property (nonatomic, readonly) ConnectorType dockConnectorType;

/*!
 @method     appleDeviceWithAppleDeviceType:
 @abstract   Returns a ProductPlanAppleDevice for the specified value of the
             AppleDeviceType enum.
 @param      type
             The apple device which the returned object is an instance of.
 @return     A ProductPlanApple device for consultation and use in the building
             of the ProductPlan certification tests.
 */
+ (id)appleDeviceWithAppleDeviceType:(AppleDeviceType)type;

/*!
 @method     initWithAppleDeviceType:
 @abstract   Returns a ProductPlanAppleDevice for the specified value of the
             AppleDeviceType enum.
 @param      type
             The apple device which the returned object is an instance of.
 @return     A ProductPlanApple device for consultation and use in the building
             of the ProductPlan certification tests.
 */
- (id)initWithAppleDeviceType:(AppleDeviceType)type;

/*!
 @method     supportedFeatureTestsForiAPVersion:
 @abstract   Returns all the FeatureTests this valid for this apple device
             for the specified version of the iAP protocol.
 @param      iAPVersion
             The protocol version for which we want the available feature tests.
 @return     An array of SelfCertTests which are supported by this
             apple device and for the specified iAP protocol.
 */
- (NSArray *)supportedFeatureTestsForiAPVersion:(iAPVersionType)iAPVersion;

/*!
 @method     supportsFeatureType:
 @abstract   Returns whether this apple device supports or not the feature.
 @param      type
             The feature which needs to have it's support verified.
 @return     Whether the feature is supported or not by this apple device,
 */
- (BOOL)supportsFeatureType:(FeatureType)type;

/*!
 @method     supportsUSBVbusPowerSource:
 @abstract   Returns whether this apple device supports or not the power source type.
 @param      powerSource
             The pwoer source which needs to have it's support verified.
 @return     Whether the power source is supported or not by this apple device,
 */
- (BOOL)supportsUSBVbusPowerSource:(USBVbusPowerSourceType)powerSource;

/*!
 @method     supportedFeatureTypesForiAPVersion:
 @abstract   Returns a set with all supported features for the iAP protocol version.
 @param      iAPVersion
             The iAP protocol for which we want the set of supported features.
 @return     A set with all supported features for this apple device for the specified
             iAP protocol.
 */
- (NSSet *)supportedFeatureTypesForiAPVersion:(iAPVersionType)iAPVersion;

/*!
 @method     supportedSelfCertTestTypesForiAPVersion:
 @abstract   Returns a set with all supported features for the iAP protocol version.
 @param      iAPVersion
             The iAP protocol for which we want the set of supported features.
 @return     A set with all supported features for this apple device for the specified
             iAP protocol.
 */
- (NSArray *)supportedSelfCertTestTypesForiAPVersion:(iAPVersionType)iAPVersionType;

/*!
 @method     supportedTransportTypes:
 @abstract   Returns a set with all supported transport types for this apple device.
 @return     A set with all supported transport types for this apple device
 */
- (NSSet *)supportedTransportTypes;

/*!
 @method     supportedUSBVbusPowerSourceTypes:
 @abstract   Returns a set with all supported power source types for this apple device.
 @return     A set of all supported power source types for this apple device.
 */
- (NSSet *)supportedUSBVbusPowerSourceTypes;

/*!
 @method     supportsConnectorType:
 @abstract   Returns whether the apple device can be used with the connector or not.
 @oaram      connector
             The connector we wish to know if the apple device can be used with.
 @return     Yes if the connector is supported or NO if it's not.
 */
- (BOOL)supportsConnectorType:(ConnectorType)connector;

/*!
 @method     supportsiAPVersionType:
 @abstract   Returns whether the apple device can be used with the specified iAP protocol or not.
 @oaram      protocol
             The iAP protocol we wish to know if the apple device can be used with.
 @return     Yes if the iAP protocol is supported or NO if it's not.
 */
- (BOOL)supportsiAPVersionType:(iAPVersionType)protocol;

@end
