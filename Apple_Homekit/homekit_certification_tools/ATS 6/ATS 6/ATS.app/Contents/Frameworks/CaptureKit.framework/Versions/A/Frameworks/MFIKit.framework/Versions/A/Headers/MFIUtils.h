//
//  MFIUtils.h
//  MFIKit
//
//  Created by Cody Brimhall on 11/30/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 Returns the bundle containing the resources that MFIKit depends on.
 
 @return The MFIKit resource bundle.
 */
NSBundle *MFIResourceBundle(void);

/**
 Returns the localized string for the given key by searching through the localization resources in the framework bundle.
 
 @param key The localization key to look up.
 @param comment The comment describing the purpose of the localized string.
 @return The localized string, or the given key if no localization can be found.
 */
NSString *MFILocalizedString(NSString *key, NSString *comment);

/**
 Returns an array of NSData objects containing the anchor certificates to be used when evaluating server trust.
 
 @return An array of NSData objects containing DER-encoded anchor certificates.
 */
NSArray *MFIAnchorCertificates(void);
