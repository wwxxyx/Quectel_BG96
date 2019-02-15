//
//  ATSiAPParser.h
//  ATSMacApp
//
//  Created by Zachary Church on 7/27/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class LKTParser;
@class LKTProtocolDefinition;

@interface ATSiAPParser : NSObject

+ (instancetype)sharedInstance;

@property (nonatomic, strong, readonly) LKTParser *iAPParser;
@property (nonatomic, strong, readonly) LKTProtocolDefinition *iAPProtocolDefinition;

@end
