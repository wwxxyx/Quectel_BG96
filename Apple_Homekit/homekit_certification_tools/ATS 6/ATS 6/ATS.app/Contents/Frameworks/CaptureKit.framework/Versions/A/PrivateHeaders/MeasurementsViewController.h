//
//  MeasurementsViewController.h
//  iAPCapture
//
//  Created by David Silver on 7/16/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <CaptureKit/MeasurementCollection.h>
#import <Grapher/Grapher.h>
#import <CaptureKit/TrafficViewController.h>

@class CaptureDocument;
@class MeasurementController;
@class CaptureEvent;

@interface MeasurementsViewController : TrafficViewController <NSTableViewDelegate, NSTableViewDataSource, NSSplitViewDelegate, GraphMultiViewDelegate> {
    MeasurementController *_measurementController;

    NSTableView *_measurementTypeTableView;
    GraphMultiView *_graphMultiView;
    NSSlider *_horizontalZoomSlider;
    NSView *_controlBar;
    NSButton *_tableDisclosureButton;
    NSTextField *_selectedTimeLabel; //Displays the time selected within the GraphMultiView

    BOOL _tableIsCollapsed; //Indicates whether or not the measurement type table view is hidden off the bottom of the screen
    BOOL _holdTimestampFieldUpdates;
    BOOL _enableAnimations;
    CGFloat _previousDividerPosition; //The position of the divider before The Collapse

    /* Holders for the MeasurementsViewMeasurementTypeContainer objects that encapsulate the objects and information about each measurement type's 
	 * representation in various parts of the history view */
    NSMutableArray *_measurementTypeContainers;  //Array of MeasurementsViewMeasurementTypeContainer objects
    NSDictionary *_containersByMeasurementTypes; //Stores MeasurementsViewMeasurementTypeContainer objects using MeasurementType strings as keys
    NSMutableArray *_selectedContainers;         //Containers of all the measurement types currently selected
}

@property (readonly) MeasurementController *measurementController;
@property (readwrite, retain) IBOutlet NSTableView *measurementTypeTableView;
@property (readwrite, retain) GraphMultiView *graphMultiView;
@property (readwrite, retain) IBOutlet NSSlider *horizontalZoomSlider;
@property (readwrite, retain) IBOutlet NSView *controlBar;
@property (readwrite, retain) IBOutlet NSButton *tableDisclosureButton;
@property (readwrite, retain) IBOutlet NSTextField *selectedTimeLabel;
@property (readonly) NSNumber *selectedTime;

- (id)initWithCaptureDocument:(CaptureDocument *)captureDocument;

- (void)reloadData;
- (void)reloadDataForMeasurementType:(NSString *)measurementType;

- (IBAction)verticalZoomIn:(id)sender;
- (IBAction)verticalZoomOut:(id)sender;
- (IBAction)expandCollapseTableView:(id)sender;
- (void)updateCollectionsUsingTimestamp:(uint64_t)timestamp;
- (void)selectClosestItemToEnteredTimestamp:(uint64_t)enteredTimestamp;
- (void)toggleDetailsShowing;

- (void)clearDisplayedEvents;
- (void)enableAnimations:(BOOL)enable;

@end
