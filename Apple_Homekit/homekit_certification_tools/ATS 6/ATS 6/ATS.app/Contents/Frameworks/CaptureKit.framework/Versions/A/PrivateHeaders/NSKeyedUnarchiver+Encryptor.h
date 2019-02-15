//
//  NSKeyedUnarchiver+Encryptor.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 10/8/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

/*!
 @header     NSKeyedUnarchiver+Encryptor
 @abstract   This header describes a category on NSKeyedUnarchiver for
             unarchiving encrypted object archives using the Encryptor class and
             a given encryption key.
 */

#import <Foundation/Foundation.h>

/*!
 @category   NSKeyedUnarchiver (Encryptor)
 @abstract   Category on NSKeyedUnarchiver for unencrypting encrypted object
             archives using the Encryptor class and a given encryption key.
 */
@interface NSKeyedUnarchiver (Encryptor)

/*!
 @method     unarchiveObjectWithData:encryptionKey:
 @abstract   Returns an decrypted and unarchived copy of the object graph 
             archive using the Encryptor class and the given encryption key.
 @param      data
             The data representing the encrypted object graph archive.
 @param      encryptionKey
             The encryption key to use for decrypting the object graph archive.
 @return     An object representing the root of the decrypted and unarchived 
             object graph.
 */
+ (id)unarchiveObjectWithData:(NSData *)data encryptionKey:(NSData *)encryptionKey;

@end
