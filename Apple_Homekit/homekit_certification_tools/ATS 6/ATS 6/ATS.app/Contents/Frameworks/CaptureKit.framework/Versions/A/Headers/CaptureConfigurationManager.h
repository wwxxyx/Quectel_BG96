//
//  CaptureConfigurationManager.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 7/28/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//
// https://gitlab.sd.apple.com/ats/CaptureKitDocumentation

/*!
 @header     CaptureConfigurationManager
 @discussion This header file describes the interface that manages attempts to 
             configure the capture configuration data model. It has methods for 
             presenting different configuration UIs to the user, as well as a 
             method for using the most recently used configuration.
 */

#import <Foundation/Foundation.h>
#import <CaptureKit/CaptureConfigurationManagerDelegate.h>
#import <CaptureKit/CaptureConfigurationTypes.h>

NS_ASSUME_NONNULL_BEGIN

// For internal debug builds, a switch to allow capture documents to be
// configured for iAP2 with the 30-pin connector using the capture configuration
// assistant. This is just a debugging tool, not a means for internal users to
// do iAP2 with the 30-pin. Disabled by default.
#if defined APPLE_INTERNAL && !defined NDEBUG
#define CC_ASSISTANT_ALLOW_30_PIN_IAP2 NO
#else
#define CC_ASSISTANT_ALLOW_30_PIN_IAP2 NO
#endif

/*!
 @const      CaptureConfigurationModeKeyPath
 @abstract   The key path for the CaptureConfigurationManager configurationMode 
             property.
 */
extern NSString *const CaptureConfigurationModeKeyPath;

/*!
 @class      CaptureConfigurationManager
 @abstract   Models the different means by which capture configuration
             information can be collected and used to configure a Capture 
             Assistant data model.
 */
@interface CaptureConfigurationManager : NSObject

/*!
 @property   configuringCapture
 @abstract   A flag indicating whether or not the capture configuration manager
             is in the process of configuring a data model.
 */
@property (nonatomic, readonly) BOOL isConfiguringCapture;

/*!
 @property   delegate
 @abstract   An object that implements the CaptureConfigurationManagerDelegate 
             protocol and wishes to receive a message when the capture 
             configuration either succeeds or fails.
 @discussion CaptureConfigurationManager must have a delegate.
 */
@property (nonatomic, assign, nullable) id<CaptureConfigurationManagerDelegate> delegate;

/*!
 Returns an instance of CaptureConfigurationManager configured with
 the given delegate object.
 
 @param      delegate
             The object that wishes to receive delegate messages when the
             capture configuration either succeeds or fails.
 @return     An instance of CaptureConfigurationManager configured with the 
             given delegate object.
 @discussion The given delegate object cannot be nil. This is the designated 
             initializer.
 */
- (instancetype)initWithDelegate:(nullable id<CaptureConfigurationManagerDelegate>)delegate;

/*!
 Presents the advanced capture configuration window
 */
- (void)configureCaptureUsingAdvancedWindow;

/*!
 Attempts to configure a capture configuration data model using the default values found in the standard user defaults store.
 */
- (void)configureCaptureUsingDefaults;

/*!
 Attempts to configure a capture configuration data model using the default values found in the input dictionary.
 
 @param configurationPlistPath Plist (Dictionary) with key/value pairs that represent the Configuration for a capture.
 */
- (void)configureCaptureContentsOfPlist:(NSString *)configurationPlistPath;

/*!
 If the receiver is in the process of configuring a capture 
 document, the configuration window is dismissed and the receiver's 
 delegate is sent a message informing it that the capture 
 configuration was canceled.
 */
- (void)cancelConfigurationShouldClose:(BOOL)shouldClose;

@end

NS_ASSUME_NONNULL_END
