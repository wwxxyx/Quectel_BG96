//
//  FindWindowController.h
//  ATSMacApp
//
//  Created by Paul Soldan on 11/5/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class CaptureDocument;

@interface FindWindowController : NSWindowController {
    IBOutlet NSComboBox *_findBox;
    IBOutlet NSButton *_previousButton;
    IBOutlet NSButton *_nextButton;
    IBOutlet NSButton *_ignoreCaseCheckBox;
    IBOutlet NSButton *_wrapAroundCheckBox;
    IBOutlet NSTextField *_statusLabel;

    NSMutableArray *_history;
    CaptureDocument *_currentCaptureDocument;

    NSString *_searchString;
    BOOL _wrapAround;
    BOOL _ignoreCase;
    NSUInteger _currentSelection;
    NSArray *_currentEvents;
}

- (IBAction)onPrevious:(id)sender;
- (IBAction)onNext:(id)sender;
- (IBAction)onJumpToSelection:(id)sender;

@property (nonatomic, retain) NSArray *currentEvents;
@property (nonatomic, retain) NSString *searchString;
@property (nonatomic, retain) NSMutableArray *history;
@property (nonatomic, retain) CaptureDocument *currentCaptureDocument;
@end
