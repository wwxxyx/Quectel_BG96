//
//  ATSHciFileImporter.h
//  ATSMacApp
//
//  Created by Edgard Lima on 10/17/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/ATSCaptureImporter.h>
#import <CaptureKit/ATSHciImporter.h>

extern NSString *ATSHciImportCompletedNotificationName;

@interface ATSHciFileImporter : ATSCaptureImporter

@property (readonly) ATSHciImporter *hciImporter;

/**
 Initializes and returns a new capture importer using the given dictionary of filename/URL pairs.
 
 The key for the files should NOT be the user's name... Rename them for what they are! Keep the extension. The "__" prefix is reserved for internal files to CaptureImporter and must not be used.
 
 @param files A dictionary whose keys and values are filenames and URLs, respectively.
 @return The new capture importer.
 */
- (instancetype)initWithFiles:(NSDictionary *)files;

/**
 Initializes and returns a new capture importer using the given dictionary of filename/URL pairs. This initializer should be used for streaming imports, as the importer will not immediately copy files from their original URLs.
 
 The "__" prefix is reserved for internal files to CaptureImporter and must not be used.
 
 @param files A dictionary whose keys and values are filenames and URLs, respectively.
 @return The new capture importer.
 */
- (instancetype)initWithFilesNoCopy:(NSDictionary *)files;

/**
 Creates and returns a new importer for the source file at the given URL. The data in the source file is pre-processed during initialization.
 
 @param url The URL to the source file.
 @return The new importer instance.
 */
- (instancetype)initWithSourceURL:(NSURL *)url;

/**
 Creates and returns a new importer with a importRoot folder.

 @param URL to the folder where the file to be imported from/exported to is placed.
 @return The new importer instance.
 */
- (instancetype)initWithURL:(NSURL *)importRoot;

/// The size in bytes of the source file.
@property (readonly) unsigned long long fileSize;

/// The current read offset in the source file This should not be used in any logic. Use the loadCompleted property to check for completion; use this for approximating import progress.
@property (nonatomic, readonly) unsigned long long fileLoadOffset;

/// The URL of the PacketLogger file to import.
@property (readonly) NSURL *url;

/// The amount of time it took to read the source file.
@property (readonly) NSTimeInterval elapsedReadTime;

/// An array of all the RFCOMM streams parsed from the source file.
@property (readonly) NSArray *rfcommStreams;

/// Sets a flag to cancel any asynchronous load in progress.
- (void)cancelLoading;

@end

