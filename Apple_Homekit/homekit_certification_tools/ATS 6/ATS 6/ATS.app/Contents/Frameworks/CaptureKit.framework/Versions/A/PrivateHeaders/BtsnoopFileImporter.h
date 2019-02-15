//
//  BtsnoopFileImporter.h
//  CaptureKit
//
//  Created by Douglas Frari on 10/4/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSHciFileImporter.h>

extern NSString * const kBtsnoopImporterFilename;

@interface BtsnoopFileImporter : ATSHciFileImporter

/**
 Creates and returns a new importer for the source file at the given URL.

 @param url The URL to the source file.
 @return The new importer instance.
 */
- (instancetype)initWithSourceURL:(NSURL *)url;

/// Perfoms asynchronous import of url initialized in initWithSourceURL.
- (BOOL)performUrlAsync;

/** Sets a flag to cancel any asynchronous load in progress. */
- (void)cancelLoading;

@end
