//
//  LKTypes.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>


enum {
    LKTFieldTypeUnknown         = 0x00,
    LKTFieldTypeUInt8           = 0x01,
    LKTFieldTypeUInt16          = 0x02,
    LKTFieldTypeUInt32          = 0x03,
    LKTFieldTypeUInt64          = 0x04,
    LKTFieldTypeSInt8           = 0x05,
    LKTFieldTypeSInt16          = 0x06,
    LKTFieldTypeSInt32          = 0x07,
    LKTFieldTypeSInt64          = 0x08,
    LKTFieldTypeBOOL            = 0x09,
    LKTFieldTypeData            = 0x0A,
    LKTFieldTypeString          = 0x0B,
    LKTFieldTypeCommandID       = 0x0C,
    LKTFieldTypeLongCommandID   = 0x0D,
    LKTFieldTypeLingoID         = 0x0E,
    LKTFieldTypeLingoIDData     = 0x0F,
    LKTFieldTypeCharData        = 0x10,
};

typedef uint8_t LKTFieldType;

enum {
    LKTSourceAccessory  = 0x00,
    LKTSourceiPod       = 0x01,
    LKTSourceUnknown    = 0xFF,
};

typedef uint8_t LKTSource;

enum {
    LKTTransportSerial          = 0x00,
    LKTTransportUSBHID          = 0x01,
    LKTTransportBluetooth       = 0x02,
    LKTTransportUSBBulkPipe     = 0x03,
    LKTTransportUnknown         = 0xFF,
};

typedef uint8_t LKTTransport;

enum {
    LKTAuthExemptNever      = 0x00,
    LKTAuthExemptSerial     = 0x01,
    LKTAuthExemptAlways     = 0x02,
    LKTAuthExemptUnknown    = 0x03,
};

typedef uint8_t LKTAuthExempt;

enum {
    LKTTransactionIDPolicyPermitted     = 0x00,
    LKTTransactionIDPolicyProhibited    = 0x01,
    LKTTransactionIDPolicyRequired      = 0x02,
    LKTTransactionIDPolicyUnknown       = 0xFF,
};

typedef uint8_t LKTTransactionIDPolicy;

enum {
    LKTPreferredBaseDec     = 0x00,
    LKTPreferredBaseHex     = 0x01,
    LKTPreferredBaseUnknown = 0xFF,
};

typedef uint8_t LKTPreferredBase;


@interface LKTTypeUtils : NSObject {}

// LKTFieldType utils
+ (NSArray *)fieldTypeNames;
+ (LKTFieldType)fieldTypeFromString:(NSString *)fieldTypeString;
+ (NSString *)stringFromFieldType:(LKTFieldType)fieldType;
+ (NSString *)hexFormatSpecifierForFieldType:(LKTFieldType)fieldType;

// LKTSource utils
+ (NSArray *)sourceNames;
+ (LKTSource)sourceFromString:(NSString *)sourceString;
+ (NSString *)stringFromSource:(LKTSource)source;

// LKTAuthExempt utils
+ (NSArray *)authExemptNames;
+ (LKTAuthExempt)authExemptFromString:(NSString *)authExemptString;
+ (NSString *)stringFromAuthExempt:(LKTAuthExempt)authExempt;

// LKTTransactionIDPolicy utils
+ (NSArray *)transactionIDPolicyNames;
+ (LKTTransactionIDPolicy)transactionIDPolicyFromString:(NSString *)transactionIDPolicyString;
+ (NSString *)stringFromTransactionIDPolicy:(LKTTransactionIDPolicy)transactionIDPolicy;

// LKTPreferredBase utils
+ (NSArray *)preferredBaseNames;
+ (LKTPreferredBase)preferredBaseFromString:(NSString *)preferredBaseString;
+ (NSString *)stringFromPreferredBase:(LKTPreferredBase)preferredBase;

// misc utils
+ (BOOL)boolFromString:(NSString *)string;

@end
