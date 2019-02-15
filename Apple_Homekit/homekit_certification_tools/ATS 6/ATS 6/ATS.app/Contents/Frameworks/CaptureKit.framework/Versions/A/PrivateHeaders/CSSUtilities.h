//
//  CSSUtilities.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 10/16/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

/*!
 The token in an HTML file that should be substituted with the name of the OS-version-specific CSS file for the current OS.
 */
extern NSString * const CSSOSToken;

/*!
 The name of the Mavericks-specific CSS file.
 */
extern NSString * const CSSMavericks;

/*!
 The name of the Yosemite-specific CSS file.
 */
extern NSString * const CSSYosemite;

/*!
 The token in an HTML file that should be substituted with the name of the Aqua-specific CSS file for the current system Aqua style.
 */
extern NSString * const CSSAquaToken;

/*!
 Returns the name of the CSS file (minus the extension) containing OS-version-specific style declarations.
 
 CSS files are bundled with the app, so the name is appropriate for use with -[NSBundle URLForResource:withExtension:].
 
 @return The name of the CSS file for the current OS version. 
 */
NSString * CSSNameForCurrentOSVersion(void);

/*!
 Returns the suffix used to distinguish the CSS used for Light or Dark system Aqua
 
 CSS files are bundled with the app, so the name is appropriate for use with -[NSBundle URLForResource:withExtension:].
 
 @return The suffix of the CSS file for the current Aqua.
 */
NSString * CSSNameForCurrentAqua(void);
