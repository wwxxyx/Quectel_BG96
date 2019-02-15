//
//  MeasurementsViewMeasurementTypeContainer.h
//  iAPCapture
//
//  Created by David Silver on 7/22/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//
// Encapsulates the information and objects used by the MeasurementsView for each measurement type
#import <Cocoa/Cocoa.h>

@class MeasurementCollection;
@class GraphView;
@class MeasurementsViewGraphController;

@interface MeasurementsViewMeasurementTypeContainer : NSObject {
    NSString *_measurementType; //The measurement type
    NSString *_measurementName; //The human-readable name of this type
    NSString *_units;
    int _idealIndex; //The index of this type relative to all other types (actual index in collections may be different
                     //if not all entries are in that collection)
    BOOL _selected;
    GraphView *_graphView;                             //The graph view representing this type
    MeasurementsViewGraphController *_graphController; //The controller object related to the graph view
    MeasurementCollection *_measurementCollection;     //The measurement collection storing the data for this type

    /* Display options */
    BOOL _shouldDisplayMinValue;
    BOOL _shouldDisplayMaxValue;
    BOOL _shouldDisplayAvgValue;
    BOOL _isBoolean;
}

@property (readwrite, retain) NSString *measurementType;
@property (readwrite, retain) NSString *measurementName;
@property (readwrite, retain) NSString *units;
@property (readwrite) int idealIndex;
@property (readwrite) BOOL selected;
@property (readwrite, assign) NSNumber *wrappedSelected; //The "selected" property wrapped in an NSNumber
@property (readwrite, retain) GraphView *graphView;
@property (readwrite, retain) MeasurementsViewGraphController *graphController;
@property (readwrite, retain) MeasurementCollection *measurementCollection;
@property (readwrite) BOOL shouldDisplayMinValue;
@property (readwrite) BOOL shouldDisplayMaxValue;
@property (readwrite) BOOL shouldDisplayAvgValue;
@property (readwrite) BOOL isBoolean;
@property (readwrite, assign) NSColor *backgroundColor;

@property (readonly) NSString *minValueString;
@property (readonly) NSString *maxValueString;
@property (readonly) NSString *avgValueString;

- (NSString *)valueStringForTime:(float)time;
@end
