//
//  BSiAP2SessionDefinition.h
//  BusSniffer
//
//  Created by Joe Basu on 11/28/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <BusSniffer/BSiAP2SessionTypes.h>

@protocol BSiAP2SessionDefinition <NSObject>

@property (nonatomic, readonly) BSiAP2SessionDefinitionStatus status;

@property (nonatomic, readonly) NSData *data;

@property (nonatomic, readonly) BOOL hasSessionID;
@property (nonatomic, readonly) BOOL hasSessionType;
@property (nonatomic, readonly) BOOL hasSessionVersion;

@property (nonatomic, readonly) UInt8 sessionID;
@property (nonatomic, readonly) UInt8 sessionType;
@property (nonatomic, readonly) UInt8 sessionVersion;

@end

@interface BSiAP2SessionDefinition : NSObject <BSiAP2SessionDefinition> {
    BSiAP2SessionDefinitionStatus _status;
    NSData *_data;
}

+ (id)sessionDefinitionWithData:(NSData *)data;

- (id)initWithData:(NSData *)data;

@end
