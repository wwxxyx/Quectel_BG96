//
//  NSKeyedArchiver+Encryptor.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 10/8/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

/*!
 @header     NSKeyedArchiver+Encryptor
 @abstract   This header describes a category on NSKeyedArchiver that provides a
             method for excrypting archivable data using the Encryptor class.
 */

#import <Foundation/Foundation.h>

/*!
 @category   NSKeyedArchiver (Encryptor)
 @abstract   Category on NSKeyedArchiver that provides a class method for 
             encrypting object archives using the Encryptor class and an 
             encryption key.
 */
@interface NSKeyedArchiver (Encryptor)

/*!
 @method     archivedDataWithRootObject:encryptionKey:
 @abstract   Returns an object graph archive for the given root object, 
             encrypted using the Encryptor class and the given encryption key.
 @param      rootObject
             The root of the object graph to be encrypted.
 @param      encryptionKey
             The encryption key to use for encrypting the archived object graph. 
             Passing nil will cause the method to raise an 
             NSInternalInconsistencyException.
 @return     A data object representing the encrypted object graph, suitable for
             serialization.
 */
+ (NSData *)archivedDataWithRootObject:(id)rootObject encryptionKey:(NSData *)encryptionKey;

@end
