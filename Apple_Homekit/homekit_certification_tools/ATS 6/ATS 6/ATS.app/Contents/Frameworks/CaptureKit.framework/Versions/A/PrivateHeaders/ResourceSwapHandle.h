//
//  ResourceSwapHandle.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 6/12/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ResourceSwapHandle : NSObject

/*!
 @method    init
 @abstract  Initialize a new swap handle
 */
- (id)init;

/*!
 @method        invalidate
 @abstract      Cleans up handle, deletes temporary file, and shuts down
 @discussion    Main queue only
 */
- (void)invalidate;

/*!
 @method        writeBlobs:
 @abstract      Write an array of blobs, returning the offset they were written to
 @discussion    Main queue only
 */
- (unsigned long long)writeBlobs:(NSArray *)blobs;

/*!
 @method        blobsAtOffset:
 @abstract      Synchronously fetch the blob array at the passed offset
 @discussion    Main queue only
 */
- (NSArray *)blobsAtOffset:(unsigned long long)offset;

/*!
 @method        blobsAtOffset:resultHandler:
 @abstact       Asynchronously fetch the blob array, passing the results to resultHandler:
 @return        A serial to be associated with this request, which will be passed back into resultHandler
 @discussion    Main queue only
 */
- (uint64_t)blobsAtOffset:(unsigned long long)offset resultHandler:(void (^)(NSArray *, uint64_t))resultHandler;

/*!
 @method        performSerially:
 @abstract      Performs a block synchronously and in serial with all other invocations of this
 */
- (void)performSerially:(void (^)(void))block;

@end
