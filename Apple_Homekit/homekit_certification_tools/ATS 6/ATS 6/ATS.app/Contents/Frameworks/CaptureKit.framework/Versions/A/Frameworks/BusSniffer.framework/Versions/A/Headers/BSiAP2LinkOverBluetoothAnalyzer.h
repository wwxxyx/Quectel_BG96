//
//  BSiAP2LinkOverBluetoothAnalyzer.h
//  BusSniffer
//
//  Created by Mark Hamlin on 1/29/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSiAPTypes.h>
#import <BusSniffer/BSiAP2LinkPacket.h>

@interface BSiAP2LinkOverBluetoothAnalyzer : NSObject

/*!
 @method    reset
 @abstract  Reset transport state
 */
- (void)reset;

/*!
 @method        processPacketLoggerFrame:
 @abstract      Process the data frame and extract relevant information
 @discussion    FUTURE. If ATS supports live captures, this is where the capture data should be placed
 Until then, stick with -processRFCOMMPayload:*
 */
//- (NSArray *)processPacketLoggerFrame:(NSData *)frame;

/*!
 @method    processRFCOMMPayload:fromSource:atTimestamp:
 @abstract  Process RFCOMM payload and return relevant
 */
- (NSArray *)processRFCOMMPayload:(NSData *)payload fromSource:(BSiAPSource)source atTimestamp:(UInt64)timestamp;

/*!
 @method    processiAP2LinkPacket:
 @abstract  Generates iAP2 messages from link packet
 */
- (NSArray *)processiAP2LinkPacket:(id <BSiAP2LinkPacket>)linkPacket;

@end
