//
//  MeasurementsViewGraphController.h
//  iAPCapture
//
//  Created by David Silver on 7/20/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Grapher/Grapher.h>

@class MeasurementCollection;
@interface MeasurementsViewGraphController : NSObject <GraphViewDataSource> {
    MeasurementCollection *_measurementCollection;
    GraphView *_graphView;

    /* Caches of the MeasurementCollection's extrema */
    float _minX;
    float _maxX;
    float _minY;
    float _maxY;

    /* Theoretical min and max values (not always used) */
    BOOL _shouldProvideTheoreticalExtrema;
    float _theoreticalMinY;
    float _theoreticalMaxY;

    /* Absolute min and max values (not always used) */
    BOOL _shouldProvideAbsoluteExtrema;
    float _absoluteMaxY;
}

- (id)initWithGraphView:(GraphView *)graphView measurementCollection:(MeasurementCollection *)measurementCollection;

- (void)reloadData;

@property (readwrite, retain) MeasurementCollection *measurementCollection;
@property (readwrite, retain) GraphView *graphView;
@property (readwrite) BOOL shouldProvideTheoreticalExtrema;
@property (readwrite) float theoreticalMinY;
@property (readwrite) float theoreticalMaxY;
@property (readwrite) BOOL shouldProvideAbsoluteExtrema;
@property (readwrite) float absoluteMaxY;
@end
