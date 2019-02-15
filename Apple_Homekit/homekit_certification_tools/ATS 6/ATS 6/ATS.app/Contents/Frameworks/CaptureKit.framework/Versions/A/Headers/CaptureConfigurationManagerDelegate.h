//
//  CaptureConfigurationManagerDelegate.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 7/28/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

/*!
 @header     CaptureConfigurationManagerDelegate
 @discussion This header describes the CaptureConfigurationManagerDelegate 
             protocol, which can be implemented by objects that are interested 
             in the results of attempts to configure capture configuration data 
             model instances.
 */

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class CaptureConfigurationManager;
@class CaptureConfiguration;

/*!
 @protocol   CaptureConfigurationManagerDelegate
 @abstract   Delegate protocol for notifying interested objects in succesful or 
             failed attempts to configure capture configuration data model 
             instances.
 */
@protocol CaptureConfigurationManagerDelegate <NSObject>

@required

/*!
 @method     captureConfigurationManager:didConfigureDataModel::
 @abstract   Sent when the capture configuration manager has successfully 
             configured the given data model.
 @param      configurationManager
             The CaptureConfigurationManager instance that configured the data 
             model.
 @param      dataModel
             The CaptureConfiguration that was successfully configured.
 @discussion This method is required.
 */
- (void)captureConfigurationManager:(CaptureConfigurationManager *)configurationManager didConfigureDataModel:(CaptureConfiguration *)dataModel;

@optional

/*!
 @method     captureConfigurationManagerDidCancel:
 @abstract   Sent when the capture configuration manager cancels an attempt to 
             configure a data model.
 @param      configurationManager
             The CaptureConfigurationManager instance that cancelled its 
             configuration attempt.
 @discussion This method is optional.
 */
- (void)captureConfigurationManagerDidCancel:(CaptureConfigurationManager *)configurationManager;

/*!
 @method     captureConfigurationManager:didFailWithError:
 @abstract   Sent when the capture configuration manager fails to configure a 
             data model.
 @param      configurationManager
             The CaptureConfigurationManager instace that failed to configure a 
             data model.
 @param      error
             An error object describing the failure.
 @discussion This method is optional.
 */
- (void)captureConfigurationManager:(CaptureConfigurationManager *)configurationManager didFailWithError:(NSError *)error;

@end

NS_ASSUME_NONNULL_END
