//
//  CaptureRuleGroupManager.h
//  ATSMacApp
//
//  Created by Natália Cabral on 30/10/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/CaptureRuleGroup.h>

@interface CaptureRuleGroupManager : NSObject

/**
 Returns all Rule group classes for ATS.
 */
@property (nonatomic, strong, readonly) NSArray<Class> *allRuleGroupClasses;

/**
 Factory method to get the singleton
 */
+ (CaptureRuleGroupManager *)sharedManager;

@end
