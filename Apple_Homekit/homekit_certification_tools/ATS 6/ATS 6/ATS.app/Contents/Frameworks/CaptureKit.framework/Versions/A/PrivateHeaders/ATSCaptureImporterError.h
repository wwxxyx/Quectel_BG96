//
//  ATSCaptureImporterError.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 3/8/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

/** Domain for errors generated when working with ATSCaptureImporter instances. */
extern NSString * const ATSCaptureImporterErrorDomain;

/** An enumeration of the possible errors when importing captured data. */
typedef NS_ENUM(NSInteger, ATSCaptureImporterError) {
    
    /** Could not use the saved configuration information to configure the importer. */
    ATSCaptureImporterSavedConfigurationError = 1,
};

/**
 Returns an error object configured for the given error code within the ATSCaptureImporterError domain.
 
 @param code The error code for the new error.
 @param underlyingError An error object to include as the underlying error in the new error's userInfo dictionary.
 @return The new error object.
 */
NSError * ATSCaptureImporterErrorWithCode(ATSCaptureImporterError code, NSError *underlyingError);
