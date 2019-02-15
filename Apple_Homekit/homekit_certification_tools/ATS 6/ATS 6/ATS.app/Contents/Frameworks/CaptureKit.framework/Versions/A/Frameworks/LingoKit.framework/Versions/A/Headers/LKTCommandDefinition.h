//
//  LKTCommandDefinition.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTDefinition.h>
#import <LingoKit/LKTTypeUtils.h>


@interface LKTCommandDefinition : LKTDefinition <NSCopying, NSCoding> {
    NSString *_name;
    uint16_t _commandID;
    LKTSource _source;
    BOOL _isResponse;
    BOOL _isACK;
    BOOL _isDeprecated;
    LKTAuthExempt _authExempt;
    LKTTransactionIDPolicy _transactionIDPolicy;
}

@property (nonatomic, readonly, copy) NSString *name;
@property (nonatomic, readonly) uint16_t commandID;
@property (nonatomic, readonly) LKTSource source;
@property (nonatomic, readonly) BOOL isResponse;
@property (nonatomic, readonly) BOOL isACK;
@property (nonatomic, readonly) BOOL isDeprecated;
@property (nonatomic, readonly) LKTAuthExempt authExempt;
@property (nonatomic, readonly) LKTTransactionIDPolicy transactionIDPolicy;


- (id)initWithName:(NSString *)name
         commandID:(uint16_t)commandID
            source:(LKTSource)source
        isResponse:(BOOL)isResponse
             isACK:(BOOL)isACK
      isDeprecated:(BOOL)isDeprecated
        authExempt:(LKTAuthExempt)authExempt
transactionIDPolicy:(LKTTransactionIDPolicy)transactionIDPolicy
          children:(NSArray *)children;

@end
