//
//  LKTRecords.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <LingoKit/LKTInstance.h>

@class LKTRecordsDefinition;

@interface LKTRecords : LKTInstance <NSCopying, NSCoding> {
    NSString *_key;
    NSArray *_records;
}

@property (nonatomic, copy) NSString *key;
@property (nonatomic, retain) NSArray *records;

@end
