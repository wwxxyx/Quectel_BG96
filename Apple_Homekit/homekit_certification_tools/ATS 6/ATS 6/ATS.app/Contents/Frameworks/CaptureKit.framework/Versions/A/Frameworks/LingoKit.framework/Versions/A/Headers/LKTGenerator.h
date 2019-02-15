//
//  LKTGenerator.h
//  LingoKit
//
//  Created by Joe Basu on 8/2/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>


@interface LKTGenerator : NSObject {
    
}

+ (NSData *)wellFormediAPPacketDataForPartialiAPPacketData:(NSData *)partialPacketData
                                              usesSyncByte:(BOOL)usesSyncByte
                                               hasSyncByte:(BOOL)hasSyncByte
                                              hasStartByte:(BOOL)hasStartByte
                                             hasLengthByte:(BOOL)hasLengthByte
                                               hasChecksum:(BOOL)hasChecksum;

@end
