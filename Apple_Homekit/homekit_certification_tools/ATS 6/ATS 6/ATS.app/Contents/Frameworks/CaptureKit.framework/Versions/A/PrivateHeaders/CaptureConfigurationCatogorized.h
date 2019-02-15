//
//  CaptureConfigurationCatogorized.h
//  CaptureKit
//
//  Created by Vinod Madigeri on 10/25/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureConfiguration.h>
/**
 Capture Configuration categories
 */
typedef NS_ENUM(NSUInteger, CaptureConfigurationCategory) {
    /// Wired capture configuration type
    CaptureConfigurationCategoryWired,
    /// Bluetooth capture configuration type
    CaptureConfigurationCategoryWireless,
};

@interface CaptureConfigurationCatogorized : CaptureConfiguration

/**
 Capture category for this configuration. Only valid if the configuration is created using the init methods defined in this file.
 */
@property (nonatomic, readwrite) CaptureConfigurationCategory category;

/**
 Returns a capture configuration data model that has been populated with the default properties from the standard user defaults store. If nothing is stored in the user defaults, the initial values are pulled in from CaptureConfigurationInitialDefaults.plist
 
 @param category Capture Configuration category that this instance is being used to configure for — Wired, BT or Wi-Fi
 
 @return A capture configuration data model that has not been populated with any of the default properties from the standard user defaults store.
 */
- (instancetype)initUsingSavedDefaultsForCategory:(CaptureConfigurationCategory)category;

/**
 Method to save the configuration dictionary to user defaults with category name as the key
 */
- (void)saveToCategorizedDefaults;

/**
 Method to load the configuration dictionary from user defaults with category name as the key
 */
- (NSMutableDictionary *)loadFromCategorizedDefaults;

@end
