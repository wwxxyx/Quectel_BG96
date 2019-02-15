//
//  LKTParser.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import <LingoKit/LKTTypeUtils.h>

@class LKTCommand;
@class LKTCommandDefinition;
@class LKTProtocolDefinition;


@interface LKTParser : NSObject {
    BOOL _iPodValidationEnabled;
    BOOL _iPodPacketReservedValueEncountered;
    BOOL _deprecatedValueEncountered;
    LKTProtocolDefinition *_protocolDefinition;
}

- (id)initWithProtocolDefinition:(LKTProtocolDefinition *)protocolDefinition
           iPodValidationEnabled:(BOOL)iPodValidationEnabled;

- (LKTCommand *)commandFromiAPPacketData:(NSData *)packetData
                                  source:(LKTSource)source
                               transport:(LKTTransport)transportType
                        hasTransactionID:(BOOL)hasTransactionID
                                   error:(NSError **)error;

@end
