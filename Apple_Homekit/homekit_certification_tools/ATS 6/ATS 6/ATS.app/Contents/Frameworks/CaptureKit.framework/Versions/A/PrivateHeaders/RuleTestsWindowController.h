//
//  RuleTestsWindowController.h
//  ATS
//
//  Created by Joe Basu on 8/3/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <Cocoa/Cocoa.h>
#import <CaptureKit/CaptureDocument.h>
#import <CaptureKit/RuleTestsDelegate.h>

@class RuleTests;

@interface RuleTestsWindowController : NSWindowController <NSTableViewDataSource, NSTableViewDelegate, RuleTestsDelegate> {
    NSInteger _countOfTotalToRun;
    NSInteger _countOfRulesRun;
    NSInteger _countOfErrors;
    NSDate *_startTime;
}

@property (nonatomic, assign) CaptureDocument *captureDocument;
@property (nonatomic, retain) IBOutlet RuleTests *ruleTests;
@property (nonatomic, retain) IBOutlet NSSearchField *ruleTestSearchField;
@property (nonatomic, retain) IBOutlet NSTableView *ruleTestListTableView;
@property (nonatomic, retain) IBOutlet NSButton *runSelectedTestsButton;
@property (nonatomic, retain) IBOutlet NSButton *runAllTestsButton;
@property (nonatomic, retain) IBOutlet NSProgressIndicator *progressIndicator;
@property (nonatomic, assign) BOOL stopTestsOnFirstError;
@property (nonatomic, assign) BOOL iterateHardwareConfigs;
@property (nonatomic) BOOL useEnterExtendedInterfaceModeInsteadOfSetUIMode;

- (IBAction)searchTests:(id)sender;
- (IBAction)runSelectedTests:(id)sender;
- (IBAction)runAllTests:(id)sender;

/*!
 @method    runAllTestsIgnoringFiltersWithCount:passed:failedOn:stopOnFirstFailure:
 @abstract  Run all tests by ignoring filters, and return test status
 @param     testCount   Pointer to value to assign the number of tests that were run
 @param     passedCount Pointer to value to assign how many tests passed
 @param     failedOn    Pointer to value to assign NSArray * (autoreleased) with an array of NSString * rule test names which failed
 @param     shouldStopOnFirstFailure    Whether or not the test run should stop after seeing the first test failure
 @return    Whether or not all tests passed
 */
- (BOOL)runAllTestsIgnoringFiltersWithCount:(NSInteger *)testCount passed:(NSInteger *)passedCount failedOn:(NSArray **)failedOn stopOnFirstFailure:(BOOL)shouldStopOnFirstFailure;

@end
