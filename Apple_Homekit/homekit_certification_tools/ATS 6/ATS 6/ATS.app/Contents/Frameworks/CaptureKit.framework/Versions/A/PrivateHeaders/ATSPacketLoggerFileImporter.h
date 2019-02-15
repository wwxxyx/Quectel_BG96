//
//  ATSPacketLoggerFileImporter.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 1/28/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSHciFileImporter.h>

extern NSString * const kPacketLoggerImporterFilename;

/** ATSPacketLoggerFileImporter extracts iAP data from RFCOMM channels found in a PacketLogger trace. */
@interface ATSPacketLoggerFileImporter : ATSHciFileImporter

#pragma mark - File Parsing State

/**
 Creates and returns a new importer for the source file at the given URL.

 @param url The URL to the source file.
 @return The new importer instance.
 */
- (instancetype)initWithSourceURL:(NSURL *)url;

/// Perfoms asynchronous import of url initialized in initWithSourceURL.
- (BOOL)performUrlAsync;

/// Sets a flag to cancel any asynchronous load in progress.
- (void)cancelLoading;

@end
