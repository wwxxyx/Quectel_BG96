//
//  EthernetOverUSBStreamReader.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/23/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//
#if APPLE_INTERNAL

#import <Foundation/Foundation.h>
#import <BusSniffer/BusSniffer.h>

@interface EthernetOverUSBStreamReader : NSObject

/*!
 @method    starkulatorStreamReader
 @abstract  Creates a stream reader for starkulator
 */
+ (EthernetOverUSBStreamReader *)starkulatorStreamReader;

/*!
 @method    nvidiaStreamReader
 @abstract  Creates a stream reader for nvidia
 */
+ (EthernetOverUSBStreamReader *)nvidiaStreamReader;

/*!
 @method    processBulkTransferData:direction:errorDescription:
 @abstract  Process the bulk transfer data in the particular direction, returning the next ethernet frame if possible
 */
- (NSData *)processBulkTransferData:(NSData *)data direction:(BSUSBTransferDirection)direction errorDescription:(NSString **)errorDescription;

@end
#endif
