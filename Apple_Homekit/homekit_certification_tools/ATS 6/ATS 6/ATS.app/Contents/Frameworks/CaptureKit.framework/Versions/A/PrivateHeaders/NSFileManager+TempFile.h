//
//  NSFileManager+TempFile.h
//  ATSMacApp
//
//  Created by Joe Basu on 9/21/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>

@interface NSFileManager (TempFile)

- (NSString *)createTemporaryPath;
- (BOOL)createTemporaryFileReturningPath:(NSString **)path fileHandle:(NSFileHandle **)fileHandle;
- (BOOL)createTemporaryFileReturningPath:(NSString **)path fileDescriptor:(int *)fileDescriptor;

@end
