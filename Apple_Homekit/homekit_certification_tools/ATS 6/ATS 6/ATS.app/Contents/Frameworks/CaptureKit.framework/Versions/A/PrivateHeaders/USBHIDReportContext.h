//
//  USBHIDReportContext.h
//  ATSMacApp
//
//  Created by Marin Balde on 12/1/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

enum {
    DescriptorIndexStateUnknown = 0,
    DescriptorIndexStateRegistered,
    DescriptorIndexStateRegisteredACKed,
    DescriptorIndexStateUnregistered,
    DescriptorIndexStateUnregisteredACKed,
};
typedef UInt8 DescriptorIndexState;

@interface USBHIDReportContext : NSObject {
    UInt16 _transactionID;
    UInt8 _index;
    DescriptorIndexState _indexState;
}
@property (nonatomic, readwrite, assign) UInt16 transactionID;
@property (nonatomic, readwrite, assign) UInt8 index;
@property (nonatomic, readwrite, assign) DescriptorIndexState indexState;

- (void)setTransactionID:(UInt16)transactionID descriptorIndex:(UInt8)index descriptorIndexState:(DescriptorIndexState)indexState;

@end

@class LKTCommand;

@interface USBHIDReportCollection : NSObject {
    NSMutableDictionary *_descriptorIndexes;
}

- (void)reset;
- (void)processRegisterDescriptorCommand:(LKTCommand *)command;
- (void)processRegisterDescriptorACKCommand:(LKTCommand *)command;
- (void)processUnregisterDescriptorCommand:(LKTCommand *)command;
- (void)processUnregisterDescriptorACKCommand:(LKTCommand *)command;

- (DescriptorIndexState)descriptorIndexStateForIndex:(UInt8)index;

@end
