//
//  GraphMultiviewContainerView.h
//  MacGraphin
//
//  Created by David Silver on 6/22/10.
//  Copyright 2010 Apple, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class GraphMultiView;
@class GraphView;
@class GraphMultiviewContainerOverlayView;
@class GraphMultiviewContainerViewRulerController;

@interface GraphMultiviewContainerView : NSView <NSAnimationDelegate> {
	GraphMultiView *_graphMultiView;
	NSMutableArray *_graphViews;
	GraphMultiviewContainerOverlayView *_overlayView;
	GraphMultiviewContainerViewRulerController *_rulerController;
	
	NSNumber *_markedX; //Using an NSNumber so nil can represent no mark
	NSColor *_markerLineColor;
	
	CGFloat _markerLineWidth; 
	
	BOOL _inMarkerLineDragOperation; //Indicates whether or not the user is currently dragging the marker line
	
	CGFloat _spaceBetweenGraphs;
	
	NSColor *_backgroundColor; 
	NSColor *_majorUnitIndicatorColor;

	NSTimeInterval _animationDuration;
	NSMutableArray *_runningAnimations;
	
	GraphView *_graphBeingAdded;
	GraphView *_graphBeingRemoved;
	GraphView *_graphBeingMoved; 
	
	BOOL _disableResizing; //For a workaround to work correctly, we need to be able to disable automatic resizing 
	BOOL _enableAnimations;
}

@property (readonly) GraphMultiView *graphMultiView;
@property (nonatomic, readwrite, retain) NSNumber *markedX;

@property (readonly) CGFloat spaceBetweenGraphs;
@property (nonatomic, readwrite) CGFloat markerLineWidth; 

@property (nonatomic, readwrite, retain) NSColor *backgroundColor;
@property (nonatomic, readwrite, retain) NSColor *majorUnitIndicatorColor;
@property (readwrite, retain) NSColor *markerLineColor;

@property (readwrite) NSTimeInterval animationDuration;

/* Setup */
- (id)initWithFrame:(NSRect)frame inGraphMultiView:(GraphMultiView *)graphMultiView;
- (void)enableAnimations:(BOOL)enable;

/* Actions */
- (void)addGraphView:(GraphView *)graphView;
- (void)insertGraphView:(GraphView *)graphView atIndex:(NSUInteger)index;
- (void)removeGraphView:(GraphView *)graphView;
- (void)moveGraph:(GraphView *)graphView toIndex:(NSUInteger)destinationIndex;
- (void)moveGraphAtIndex:(NSUInteger)originIndex toIndex:(NSUInteger)destinationIndex;
- (void)markX:(CGFloat)xValue;
- (void)unmarkX;
- (void)centerOnX:(CGFloat)xValue; 

/* Resizing */
- (void)adjustTimeRuler;
- (void)resizeViewAndSubviewsToNewWidth:(CGFloat)newWidth;
- (void)resizeToOptimalWidth;
- (void)layoutGraphs;

/* Calculators 
 * A "graphX" refers to a point in the graph's "real-world" units (such as seconds) */
- (CGFloat)coordinateForGraphX:(CGFloat)xValue; 
- (CGFloat)graphXForCoordinate:(CGFloat)coordinate;
- (CGFloat)widthForGraphX:(CGFloat)maxValue withMin:(CGFloat)minValue unitWidth:(CGFloat)unitWidth margin:(CGFloat)margin; //Returns the minimum frame width of a graph able to display the provided value, given the provided scale 
- (BOOL)xIsWithinBounds:(CGFloat)xValue;
- (CGFloat)constrainedX:(CGFloat)xValue;

@end
