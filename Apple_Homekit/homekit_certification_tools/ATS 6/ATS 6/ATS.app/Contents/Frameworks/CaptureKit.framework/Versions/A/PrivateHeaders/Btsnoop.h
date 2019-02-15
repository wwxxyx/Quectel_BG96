//
//  Btsnoop.h
//  BTSnoop file format support.
//
//  Created by Edgard Lima on 09/12/18.
//  Copyright (c) 2018 Apple. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/BtsnoopRecord.h>
#import <CaptureKit/BtsnoopTypes.h>
#import <ATSImportKit/ATSImportLogger.h>

@interface Btsnoop : NSObject

@property NSURL * url;

/// BtSnoop file format version. See kBtSnoopVersion.
@property UInt32 fileVersion;

/// The type of datalink header used in the packet records.
@property BtsnoopDataLinkType datalinkType;

/// Setting this property to True will interrupt the file loading/parsing process. See loadFromFileHandle, loadFromFilePath and nextRecord.
@property BOOL cancelLoading;

/// The whole file size. The loading progress could be calculated with fileOffset * 100.0% / fileSize. See writeHeaderToFileHandle, loadFromFilePath and nextRecord.
@property (readonly) NSUInteger fileSize;

/// The current offset in the loading/parsing process. The loading progress could be calculated with fileOffset * 100.0% / fileSize. See writeHeaderToFileHandle, loadFromFilePath and nextRecord.
@property (readonly) NSUInteger fileOffset;

@property (readonly) NSFileHandle * fileHandle;

/// Writes a valid BtSnoop file header. See http://www.fte.com/webhelp/bpa600/Content/Technical_Information/BT_Snoop_File_Format.htm
+ (BOOL)writeHeaderToFileHandle:(NSFileHandle *)fileHandle log:(ATSImportLogger*)log;

/// Parses the BtSnoop file header and gets ready to start parsing records.
- (BOOL)loadFromFileHandle:(NSFileHandle *)fileHandle log:(ATSImportLogger*)log;

/// Parses the BtSnoop file header and gets ready to start parsing records.
- (BOOL)loadFromFilePath:(NSURL *)url log:(ATSImportLogger*)log;

/// Close the file handle and gets ready to a new call to loadFromFileHandle and loadFromFilePath.
- (void)reset;

/// default constructor.
- (instancetype)init;

// TODO anywhere you see log, call it logger.
/// Seek back to the first record. Can just be called after loadFromFileHandle or loadFromFilePath. Use this method if you have already called nextRecord, and want to read each record again from the beginnig. May raise an exception.
- (BOOL)seekToFirstRecord:(ATSImportLogger*)log;

/// Get the next record. Can just be called after loadFromFileHandle or loadFromFilePath. Return nil if there isn't more records to read.
- (BtsnoopRecord*)nextRecord:(ATSImportLogger*)log;

@end
