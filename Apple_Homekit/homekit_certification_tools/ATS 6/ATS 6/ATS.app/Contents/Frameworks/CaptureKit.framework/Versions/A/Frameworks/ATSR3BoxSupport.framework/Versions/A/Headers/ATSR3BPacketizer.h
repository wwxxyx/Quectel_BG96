//
//  ATSR3BPacketizer.h
//  ATSR3BoxSupport
//
//  Created by Marin Balde on 4/16/12.
//  Copyright © 2012 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <ATSR3BoxSupport/ATSR3BoxTypes.h>

@class ATFR3Frame;

@interface ATSR3BPacketizer : NSObject 

/**
 Create an instance of ATSR3 packetizer.
 @param productName Name of the Capture device (ATS Lightning Box or ATS Smart Connector Box) to be used as source in an ATFR3Frame
 @param productID ID of the Capture device (ATS Lightning Box or ATS Smart Connector Box) to be used as source in an ATFR3Frame
 */
- (instancetype)initWithProductName:(NSString *)productName andProductID:(NSUInteger)productID;

- (ATFR3Frame *)processATSR3BoxByte:(UInt8)byte;

+ (NSData *)atfR3DataWithCommand:(ATFR3Command)command
                         payload:(const UInt8 *)payload
                     payloadSize:(UInt32)payloadSize;

@end
