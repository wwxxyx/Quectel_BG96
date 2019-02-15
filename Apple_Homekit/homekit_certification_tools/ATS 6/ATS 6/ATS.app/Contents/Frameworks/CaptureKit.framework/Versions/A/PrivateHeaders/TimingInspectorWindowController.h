//
//  TimingInspectorWindowController.h
//  ATS
//
//  Created by Joe Basu on 3/23/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <Cocoa/Cocoa.h>

@interface TimingInspectorWindowController : NSWindowController {
    UInt64 _startItemTimestamp;
    UInt64 _endItemTimestamp;

    NSTextField *_startItemDescriptionTextField;
    NSTextField *_startItemExtendedDescriptionTextField;

    NSTextField *_endItemDescriptionTextField;
    NSTextField *_endItemExtendedDescriptionTextField;

    NSTextField *_computedDeltaTextField;
    NSTextField *_computedDeltaUnitsTextField;
}

+ (TimingInspectorWindowController *)sharedTimingInspectorWindowController;

@property (nonatomic) UInt64 startItemTimestamp;
@property (nonatomic) UInt64 endItemTimestamp;

@property (nonatomic, retain) IBOutlet NSTextField *startItemDescriptionTextField;
@property (nonatomic, retain) IBOutlet NSTextField *startItemExtendedDescriptionTextField;

@property (nonatomic, retain) IBOutlet NSTextField *endItemDescriptionTextField;
@property (nonatomic, retain) IBOutlet NSTextField *endItemExtendedDescriptionTextField;

@property (nonatomic, retain) IBOutlet NSTextField *computedDeltaTextField;
@property (nonatomic, retain) IBOutlet NSTextField *computedDeltaUnitsTextField;

- (void)resetStartItemDescriptions;
- (void)resetEndItemDescriptions;
- (void)timestampUnitsChanged:(id)notification;

@end
