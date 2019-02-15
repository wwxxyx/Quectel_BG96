//
//  iAP1PacketEvent.h
//  ATS
//
//  Created by Joe Basu on 9/9/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/CaptureEvent.h>

@class BSiAPPacket;
@class LKTCommand;

@interface iAP1PacketEvent : CaptureEvent

@property (nonatomic, strong) BSiAPPacket *packet;
@property (nonatomic, readonly, strong) LKTCommand *parsedCommand;
@property (nonatomic, strong) NSError *error;

+ (id)iAP1PacketEventWithBSiAPPacket:(BSiAPPacket *)packet;
- (id)initWithBSiAPPacket:(BSiAPPacket *)packet;

- (NSString *)sourceDescription;
- (NSString *)lingoDescription;
- (NSString *)commandDescription;
- (NSString *)transactionIDDescription;
- (NSString *)payloadDescription;
- (NSString *)rawBytes;

/*!
 @method    classificationForCommandWithLingo:command:
 @abstract  Returns the type classification for a packet event with the given parsed lingo and command
 */
+ (id)classificationForCommandWithLingo:(UInt8)lingoID command:(UInt16)commandID;

@end
