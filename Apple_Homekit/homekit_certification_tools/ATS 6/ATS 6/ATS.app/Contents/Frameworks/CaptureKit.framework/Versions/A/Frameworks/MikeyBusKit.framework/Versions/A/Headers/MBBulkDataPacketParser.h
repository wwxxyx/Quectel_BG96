//
//  MBBulkDataPacketParser.h
//  MikeyBusKit
//
//  Created by Wesley McCloy on 11/19/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

/*!
 @header        MBBulkDataPacketParser
 @discussion    This header defines the functions and properties used to interface with a BulkData Packet Parsing operation.
 */

#import <Foundation/Foundation.h>
#import <MikeyBusKit/MBBulkDataPacket.h>

@interface MBBulkDataPacketParser : NSObject

/**
 Direction this bulk data parser is configured for.
 */
@property (nonatomic, readonly) MBBulkDataPacketDirectionType direction;

/*!
 Initialization method to be used. 
 
 @param direction Dictates the source direction of packets that will be returned by this parser. Whether it's from Host to Device (DownLink) or Device to Host (UpLink)
 */
- (instancetype)initWithDirection:(MBBulkDataPacketDirectionType)direction;

/*!
 Runs the bulk data parsing operation with the given data parameter.
 
 @param data A data of 2 bytes as the input data to the parsing. (must be undecoded 16 bits)
 @return A complete parsed bulk data
 */
- (NSData *)processBulkDataBytes:(uint16_t)dataBytes;

/**
 Resets
 */
- (void)reset;

@end
