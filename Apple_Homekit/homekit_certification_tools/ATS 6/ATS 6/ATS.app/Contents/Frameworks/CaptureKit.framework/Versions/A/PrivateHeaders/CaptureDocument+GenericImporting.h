//
//  CaptureDocument+GenericImporting.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/7/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureDocument.h>

@class IKImportPacket;
@class ATSCaptureImporter;

@interface CaptureDocument ()

@property (nonatomic, retain) NSArray *captureImporters;

@end

@interface CaptureDocument (GenericImporting)

/*!
 @method        captureDocumentWithImporter:headless:streaming:
 @abstract      Create, configure, and import data to a new capture document. If headless, this will not return until the import has completed
 */
+ (CaptureDocument *)captureDocumentWithImporter:(ATSCaptureImporter *)importer headless:(BOOL)isHeadless streaming:(BOOL)isStreaming;

/*!
 @method    writeImportedDocumentToURL:error:
 @abstract  Writes a copy of the imported document to the passed URL
 */
- (BOOL)writeImportedDocumentToURL:(NSURL *)url error:(NSError **)error;

#pragma mark -
#pragma mark Manual Importing

/*!
 @method    beginImportWithDataModel:display:useProgressSheet:error:
 @abstract  Convenience method to begin import, calling everything you need to setup the new CaptureDocument
 */
- (BOOL)beginImportWithDataModel:(CaptureConfiguration *)dataModel
                         display:(BOOL)display
                useProgressSheet:(BOOL)useProgressSheet
                        filename:(NSString *)filename
                           error:(NSError **)error;

/*!
 @method    importPacket:
 @abstract  Import a packet info dictionary. This may be called from any thread.
 */
- (void)importPacket:(IKImportPacket *)packet;

/*!
 @method    updateImportProgress:
 @abstract  Updates the import with a progress (between 0.0 and 1.0)
 */
- (void)updateImportProgress:(float)progress;

/*!
 @method    endImport
 @abstract  Completes the import, closing the panel
 */
- (void)endImport;

@end
