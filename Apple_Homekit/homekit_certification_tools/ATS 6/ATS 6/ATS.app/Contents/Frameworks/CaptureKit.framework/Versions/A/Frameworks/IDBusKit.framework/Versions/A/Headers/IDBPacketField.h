//
//  IDBPacketField.h
//  IDBusKit
//
//  Created by Mark Hamlin on 7/30/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class IDBPacketField;
IDBPacketField *IDB_PACKET_FIELD(NSString *key, NSObject *value);
IDBPacketField *IDB_PACKET_FIELD_UINT_DEC(NSString *key, UInt64 value);
IDBPacketField *IDB_PACKET_FIELD_UINT_BOOL(NSString *key, BOOL value);
#define IDB_PACKET_FIELD_FMT(key, valueFmt, ...) [IDBPacketField fieldWithKey:key rawValue:[NSString stringWithFormat:valueFmt, ##__VA_ARGS__] displayValue:[NSString stringWithFormat:valueFmt, ##__VA_ARGS__]]
#define IDB_PACKET_FIELD_UINT8_HEX(key, valueNum) [IDBPacketField fieldWithKey:key rawValue:@(valueNum) displayValue:[NSString stringWithFormat:@"0x%02x", (UInt8)valueNum]]
#define IDB_PACKET_FIELD_UINT64_HEX(key, valueNum) [IDBPacketField fieldWithKey:key rawValue:@(valueNum) displayValue:[NSString stringWithFormat:@"0x%qx", (UInt64)valueNum]]
#define IDB_PACKET_FIELD_ERROR_CODE(errorCode) IDB_PACKET_FIELD_FMT(@"errCode", @"0x%02X (%@)", (errorCode), IDBStringFromErrorCode((errorCode)))
IDBPacketField *IDB_PACKET_FIELD_BULK_DATA_PAYLOAD_LENGTH(NSString *key, unsigned valueNum);

@interface IDBPacketField : NSObject

/*!
 Key for the field.
 */
@property (nonatomic, readonly, strong) NSString *key;

/*!
 String value for the field.
 */
@property (nonatomic, readonly, strong) NSString *displayValue;

@property (nonatomic, readonly, strong) NSObject *rawValue;

/*!
 Returns instance with the key and value.
 */
+ (instancetype)fieldWithKey:(NSString *)key rawValue:(NSObject *)rawValue displayValue:(NSString *)displayValue;

@end
