//
//  LKTCommand.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTFieldContainer.h>


@class LKTField;
@class LKTProtocolDefinition;
@class LKTCommandDefinition;

@interface LKTCommand : LKTFieldContainer <NSCopying, NSCoding> {
    NSString *_name;
    BOOL _hasLingoID;
    uint8_t _lingoID;
    BOOL _hasCommandID;
    uint16_t _commandID;
    BOOL _hasTransactionID;
    uint16_t _transactionID;
    uint16_t _adjustedPayloadLength;
    BOOL _containsDeprecatedValues;
}

@property (nonatomic, copy) NSString *name;
@property (nonatomic) BOOL hasLingoID;
@property (nonatomic) uint8_t lingoID;
@property (nonatomic) BOOL hasCommandID;
@property (nonatomic) uint16_t commandID;
@property (nonatomic) BOOL hasTransactionID;
@property (nonatomic) uint16_t transactionID;
@property (nonatomic) uint16_t adjustedPayloadLength;
@property (nonatomic) BOOL containsDeprecatedValues;

- (NSString *)evaluateTemplateUsingProtocolDefinition:(LKTProtocolDefinition *)protocolDefinition;

@end
