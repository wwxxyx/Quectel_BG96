//
//  ATSR3TestWindowController.h
//  ATSMacApp
//
//  Created by Marin Balde on 4/11/12.
//  Copyright © 2012 Apple Inc. All Rights Reserved.
//

#import <Cocoa/Cocoa.h>

@class PhysicalATFR3CaptureDevice;
@class CaptureEvent;
@class UARTATFR3TrafficViewController;

@interface ATSR3TestWindowController : NSWindowController <NSTableViewDataSource, NSTableViewDelegate> {
    BOOL (^_frameFilterBlock)(CaptureEvent *event);
}
@property (nonatomic, assign) PhysicalATFR3CaptureDevice *atsR3BoxCaptureDevice;
@property (nonatomic, assign) UARTATFR3TrafficViewController *uartATFR3TrafficViewController;
@property (nonatomic, retain) IBOutlet NSSearchField *commandSearchField;
@property (nonatomic, retain) IBOutlet NSTableView *commandListTableView;
@property (nonatomic, retain) IBOutlet NSButton *sendCommandButton;
@property (nonatomic, retain) IBOutlet NSTextField *payloadTextField;
@property (nonatomic, retain) IBOutlet NSTextField *rawBytesLabel;
@property (nonatomic, readwrite, assign) BOOL frameFilterEnabled;

- (IBAction)searchCommands:(id)sender;
- (IBAction)sendCommandButtonPressed:(id)sender;

@end
