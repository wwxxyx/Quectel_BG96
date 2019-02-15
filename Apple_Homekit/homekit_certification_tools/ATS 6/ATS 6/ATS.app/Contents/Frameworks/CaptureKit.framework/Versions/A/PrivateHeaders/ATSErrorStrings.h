//
//  ATSErrorStrings.h
//  ATS
//
//  Created by Marin Balde on 7/14/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

/** ATSErrorStrings provides a singleton interface to the store of rule error strings used by ATS. */
@interface ATSErrorStrings : NSObject

/**
 Returns the shared store of rule error strings.
 
 @return The error strings store, or nil if the strings could not be loaded.
 */
+ (ATSErrorStrings *)sharedInstance;

/**
 Returns the rule error string associated with the given key.
 
 @param key The key for the desired error string.
 @return The error string, or nil if the key does not exist in the store.
 */
- (NSString *)stringForKey:(NSString *)key;

@end
