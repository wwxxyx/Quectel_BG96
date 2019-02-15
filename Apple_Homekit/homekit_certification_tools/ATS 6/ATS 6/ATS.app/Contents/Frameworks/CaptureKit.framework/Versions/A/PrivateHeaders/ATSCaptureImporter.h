//
//  ATSCaptureImporter.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/9/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <ATSImportKit/ATSImportLogger.h>

@class CaptureConfiguration;
@class IKImportPacket;
@class CaptureDocument;

/** The import configuration key for the flag that indicates whether the source data is from a passive Bluetooth capture. */
static NSString * const ATSIsPassiveCaptureKey = @"isPassiveCapture";

/** The import configuration key for the flag that indicates whether the source data contains iAP1 traffic. */
static NSString * const ATSContainsiAP1DataKey = @"containsiAP1Data";

/** The import configuration key for the index of the RFCOMM data stream that contains the iAP data to import. */
static NSString * const ATSRFCOMMStreamIndexKey = @"streamIndex";

// Obsolete configuration keys from ATS 5.6 and earlier
static NSString * const ATS_5_6_TraceFromBTCT = @"traceFromBTCT";
static NSString * const ATS_5_6_TraceUsesiAP2 = @"traceUsesiAP2";


@interface ATSCaptureImporter : NSObject


/** The document receiving the imported data. */
@property (nonatomic, readonly, weak) CaptureDocument *document;

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
 Deliver the imported packet from performImport*.
 
 @param packet The imported packet to deliver.
 */
- (void)deliverImportedPacket:(IKImportPacket *)packet;

/**
 Deliver the imported packet from performImport*.
 
 @param packet the imported packet to deliver.
 @param timestampSince1970 The packet timestamp, in Nanoseconds, since 1970. (system timestamp)
 */
- (void)deliverImportedPacket:(IKImportPacket *)packet absoluteTimestampSince1970:(UInt64)timestampSince1970;

/**
 Updates document's progress indicator with the given progress value.
 
 @param progress A floating-point value in the range [0, 1].
 */
- (void)updateImportProgress:(float)progress;

#pragma mark - File Access

/**
 Returns a readable file handle pointing to the import file with the given name.
 
 @param fileName The name of the import file for which to returna handle.
 @return A readable file handle.
 */
- (NSFileHandle *)fileHandleWithName:(NSString *)fileName;

/**
 Returns the contents of the import file with the given name.
 
 @param fileName The name of the import file whose contents should be returned.
 @return The data contained in the import file.
 */
- (NSData *)fileContentsWithName:(NSString *)fileName;

/**
 Returns a file URL object for the import file with the given name.
 
 @param fileName The name of the import file for which to return a URL.
 @return The import file URL.
 */
- (NSURL *)fileURLWithName:(NSString *)fileName;

#pragma mark - Subclass Overrides

/** The configuration dictionary for the import which the subclassers should generate from its properties. These should be restored later in configureDataModelForImport:*. */
@property (nonatomic, readonly) NSDictionary *configuration;

/** The fileName to use for display in the UI. */
@property (nonatomic, readonly) NSString *displayFilename;

/** If there are errors imporint, it will be set here any */
@property (nonatomic, readonly) ATSImportLogger *logger;

/**
 Override to set properties on the passed data model so that CaptureDocument may have the necessary configuration to handle any future import packets. Also restores our own properties based on the configuration dictionary.
 
 The default implementation does nothing.
 
 @param dataModel The capture configuration data model to use when configuring the capture document.
 @param configuration A dictionary of configuration options used to interpret the imported data.
 @param error On return, if an error occurred this will point to a valid error object describing the problem. Pass nil if no error information is needed.
 @return YES if the data model was configured successfully; NO otherwise.
 */
- (BOOL)configureDataModelForImport:(CaptureConfiguration *)dataModel configuration:(NSDictionary *)configuration error:(NSError **)error;

/**
 Override to import the document synchronously.
 
 The default implementation does nothing.
 
 @param error On return, if an error occurred this will point to a valid error object describing the problem. Pass nil if no error information is needed.
 @return YES if the import was successful; NO otherwise.
 */
- (BOOL)performImportWithError:(NSError **)error;

/**
 For streaming imports, this gives the subclass an opportunity to synchronize their file handles before they are read from.
 
 The default implementation does nothing.
 */
- (void)synchronizeFiles;

#pragma mark - Document

/**
 Initialize and returns a new capture importer configured for the given URL.
 
 CaptureDocument calls this when loading a trace from disk.
 
 @param importRoot The root of the directory containing the data to import.
 @return The new capture importer.
 */
- (instancetype)initWithURL:(NSURL *)importRoot;

/**
 Writes imported data to the given URL.
 
 @param url The URL to which to write the imported data.
 @param error On return, if an error occurred this will point to a valid error object. Pass nil if no error information is required.
 @return YES if the imported data was written successfully; NO otherwise.
 */
- (BOOL)writeToURL:(NSURL *)url error:(NSError **)error;

/**
 Imports data to the given document synchronously.
 
 @param document The capture document that should receive the imported data.
 @param A flag indicating whether or not the import is being performed with a UI.
 @param error On return, if an error occurred this will point to a valid error object. Pass nil if no error information is required.
 @return YES if the import was successful; NO otherwise.
 */
- (BOOL)performImportUsingDocument:(CaptureDocument *)document isHeadless:(BOOL)isHeadless error:(NSError **)error;

/**
 Configures the data model and also gives the subclass an opportunity to restore all properties.
 
 @param dataModel The data model to configure.
 @param error On return, if an error occurred this will point to a valid error object. Pass nil if no error information is required.
 @return YES if the configuration was successful; NO otherwise.
 */
- (BOOL)configureDataModelForImport:(CaptureConfiguration *)dataModel error:(NSError **)error;


@end
