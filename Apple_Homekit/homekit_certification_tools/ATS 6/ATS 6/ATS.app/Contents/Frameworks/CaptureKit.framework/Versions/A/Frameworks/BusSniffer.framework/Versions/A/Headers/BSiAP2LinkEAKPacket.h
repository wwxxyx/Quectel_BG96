//
//  BSiAP2LinkEAKPacket.h
//  BusSniffer
//
//  Created by Joe Basu on 11/28/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <BusSniffer/BSiAP2LinkPacket.h>

@protocol BSiAP2LinkEAKPacket <BSiAP2LinkPacket>

@property (nonatomic, readonly) NSUInteger acknowledgementNumberCount;
@property (nonatomic, readonly) NSArray *acknowledgementNumbers;

@end

@interface BSiAP2LinkEAKPacket : BSiAP2LinkPacket <BSiAP2LinkEAKPacket> {
    NSArray *_acknowledgementNumbers;
}

@end
