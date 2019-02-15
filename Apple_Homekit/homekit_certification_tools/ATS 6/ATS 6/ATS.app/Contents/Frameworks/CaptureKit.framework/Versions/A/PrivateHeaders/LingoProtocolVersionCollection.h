//
//  LingoProtocolVersionCollection.h
//  ATSMacApp
//
//  Created by Paul Soldan on 10/26/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ProtocolVersion : NSObject {
    UInt8 _majorVersion;
    UInt8 _minorVersion;
}

@property (nonatomic, readwrite, assign) UInt8 majorVersion;
@property (nonatomic, readwrite, assign) UInt8 minorVersion;
- (id)initWithMajor:(UInt8)majorVersion minor:(UInt8)minorVersion;

@end

@interface LingoProtocolVersionCollection : NSObject {
    NSMutableDictionary *_protocolVersionsByLingo;
    NSMutableDictionary *_lingoesPending;
}

- (void)setPending:(BOOL)pending forLingo:(UInt8)lingo;
- (NSArray *)lingoesPending;
- (ProtocolVersion *)protocolVersionForLingo:(UInt8)lingo;
- (void)setProtocolVersion:(ProtocolVersion *)protocolVersion forLingo:(UInt8)lingo;
- (void)setProtocolVersionMajor:(UInt8)majorVersion minor:(UInt8)minorVersion forLingo:(UInt8)lingo;
- (void)resetLingoProtocolVersions;

@end
