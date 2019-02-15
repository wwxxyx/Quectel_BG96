//
//  AppStenograph.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 4/25/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/StenographModule.h>

#import <CaptureKit/ProductPlanSelfCertTest.h>

@interface AppStenograph : StenographModule

/*!
 @property  currentCATestJustFinished
 @abstract  Set when a CA test has just finished.
 */
@property (nonatomic, readonly, assign) BOOL currentCATestJustFinished;

/*!
 Contains name of tests which finished.
 */
@property (nonatomic, readonly, strong) NSMutableArray *currentCATestJustFinishedTypes;

/*!
 @property  appleTrademarks
 @abstract  Array of strings representing Apple trademarks.
 @discussion Trademarks generated from http://www.apple.com/legal/trademark/appletmlist.html
 */
@property (nonatomic, readonly) NSArray *appleTrademarks;

@end
