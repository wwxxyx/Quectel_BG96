//
//  GraphMultiView.h
//  MacGraphin
//
//  Created by David Silver on 6/22/10.
//  Copyright 2010 Apple, Inc. All rights reserved.
//
//  

/*! @header GraphMultiView 
	@abstract GraphMultiView class and associated protocols */

#import <Cocoa/Cocoa.h>
#import "GraphView.h"

@class GraphView;
@class GraphMultiView;
@class GraphMultiviewContainerView;
@class DSCollectionViewController;
@class DSScrollView;

@protocol GraphMultiViewDelegate;
@protocol GraphViewDataSource;

/*! @class GraphMultiView
	@abstract Displays multiple GraphViews with the same x-axis
	@discussion A GraphMultiView is used to display multiple GraphViews with the same x-axis. The GraphMultiView ensures that all of the GraphViews have the same
				minimum value, maximum value, and scale. When using a GraphMultiView, it is essential that you change all of these propreties through the GraphMultiView, and
				not by accessing the GraphView directly. 
 
				The GraphViews are contained within an NSScrollView, which allows them to be wider than would ordinarily fit on the screen. If in scaled mode, all graphs within the 
				GraphMultiView will be sized so that they fill the scroll view's content area or so that they take up as much space as necessary to display all of the points at the current
				scale level, whichever is larger. If a graph gains a new point that exceeds the current size, the GraphMultiView will automatically increase the width of all of the graphs
				to accomodate the new point. If the horizontal scroll bar is scrolled all the way to the right before the resize, it will autoscroll so that it remains all the way to the right 
				after the resize (if the scroll bar is positioned anywhere else, it will not autoscroll). If the GraphMultiView is in fixed mode, the graphs will share the same minimum and maximum
				x and y values, and will be sized so that they take up the entire content area of the scroll view.
 
				Optionally, the GraphMultiView can display a label alongside each GraphView. To display labels, set the labelViewsVisible property to YES. By default, labels consist of a simple text
				field populated with the value of each GraphView's title property, and have a background color the same as the graph's. For more advanced labels, use the initWithFrame:labelViewControllerClass:
				method to initialize your GraphMultiView. The labelViewControllerClass is the Class object for an NSViewController subclass that will create and control your custom label view. 
				An instance of the labelViewControllerClass will be created for each GraphView using the init method, so the view controller's init method must be able to create its view without
				any additional parameters. Your labelViewControllerClass should implement an idealSize method that returns an NSSize representing the minimum size the label view needs to display 
				its data, taking into account variables such as the length of the title string. The actual size of the label view may be larger, so the label view should resize appropriately. 
				You may subclass the default label view controller and label view, GraphMultiViewLabelViewController and GraphMultiViewLabelView, to provide starting points for your labels,
				although this is not necessary. 
 
				Many of the properties and behavior of the GraphMultiView mirror that of the GraphView. For further information about these settings, please see GraphView.h */
 
@interface GraphMultiView : NSView <GraphViewDelegate> {
	/* Graph Superviews */
	DSScrollView *_containerScrollView; //The scroll view enclosing the container view
	GraphMultiviewContainerView *_containerView;
	
	/* Label related variables */
	DSCollectionViewController *_labelCollectionViewController;
	Class _labelViewControllerClass; //The Class of the view controller for each label object
	BOOL _labelViewsVisible;
	BOOL _doNotResizeLabels; //Indicates that we shouldn't attempt to resize labels any more
	float _labelCollectionViewWidth; //How wide the labelCollectionView should be
	
	NSMutableArray *_graphViews;
	id <GraphMultiViewDelegate> _delegate;
	
	/* Variables that need to be consistent across all of the graphs */
	BOOL _hasFixedExtrema; //Should the graphs be in fixed mode?
	BOOL _hasFixedXMin; //Should the minimum x value of the graphs be fixed? 
	BOOL _hasFixedYMax; //Should the maximum y value of the graphs be fixed? 
	
	CGFloat _xAxisMargin; //pixels
	CGFloat _yAxisMargin; //pixels
	
	CGFloat _xAxisScale; //multiplier for base
	CGFloat _yAxisScale; //multiplier for base
	CGFloat _minGraphHeight; //The minimum height a graph can be 

	CGFloat _xMin; 
	CGFloat _xMax;
	CGFloat _yMin;
	CGFloat _yMax;
	
	/* Zero graph UI elements */
	NSTextField *_zeroGraphsLabel;
	
	/* View attributes */
	NSColor *_backgroundColor;
	NSTimeInterval _animationDuration; //Duration of animations within the MultiView
}

/*! @property scrollView
	@abstract The GraphMultiView's scroll view */
@property (readonly) NSScrollView *containerScrollView; 

/*! @property containerView
	@abstract The GraphMultiView's containter view 
	@discussion The container view is responsible for arranging the GraphViews contained within the MultiView. You will rarely have any need 
				to access this directly. */
@property (readonly) GraphMultiviewContainerView *containerView;

/*! @property delegate
	@abstract The GraphMultiView's delegate
	@discussion The delegate is notified when various events occur within the GraphMultiView. */
@property (readwrite, assign) id <GraphMultiViewDelegate> delegate;

/*! @property labelViewsVisible 
	@abstract Indicates whether to show or hide the label views 
	@discussion Note that no labels will be displayed when there aren't any graphs */
@property (nonatomic, readwrite) BOOL labelViewsVisible;

/*! @property graphViews
	@abstract The graphViews contained within the GraphMultiView
	@discussion To add or remove GraphViews from the MultiView, use the methods in the GraphMultiView class. */
@property (readonly) NSArray *graphViews;

/*! @property numberOfGraphs
	@abstract The number of graphs currently container within the GraphMultiView */
@property (readonly) NSUInteger numberOfGraphs;

/*! @property selectedX
	@abstract The currently selected x value 
	@discussion When an x value is selected, it has a line drawn vertically through it across all graphs. A small marker is also
				displayed above the scroll view's ruler. By default, the user can select an x value by clicking on the graph or the 
				ruler. */
@property (readonly) NSNumber *selectedX;

/*! @group Display*/
/*! @property hasFixedExtrema
	@abstract Specifies whether or not the GraphViews contained within the GraphMultiView should have fixed extrema
	@discussion For further discussion of this property, see the documentation for GraphView */
@property (nonatomic, readwrite) BOOL hasFixedExtrema;
/*! @property hasFixedXMin
 @abstract Specifies whether or not the GraphViews contained within the GraphMultiView should have fixed minimum x value
 @discussion For further discussion of this property, see the documentation for GraphView */
@property (nonatomic, readwrite) BOOL hasFixedXMin;

/*! @property hasFixedYMax
 @abstract Specifies whether or not the GraphViews contained within the GraphMultiView should have fixed maximum y value
 @discussion For further discussion of this property, see the documentation for GraphView */
@property (nonatomic, readwrite) BOOL hasFixedYMax;

/*! @property xAxisScale
	@abstract The x axis scale of the GraphViews contained within the MultiView. 
	@discussion If in scaled mode, this is must be set manually. If in fixed mode, this is calculated automatically, which overrides any 
				value you manually set. If the MultiView doesn't contain any graphs, this returns -1. */
@property (readwrite) CGFloat xAxisScale;
/*! @property yAxisScale
	@abstract The y axis scale of the GraphViews contained within the MultiView. 
	@discussion The y axis scale alters the height of all of the GraphViews within the GraphMultiView. */
@property (nonatomic, readwrite) CGFloat yAxisScale;

/*! @property minGraphHeight
	@abstract The minimum allowed height of an individual graph 
	@discussion This puts a cap on the minimum yAxisScale so that the graphs never get smaller than this value */
@property (nonatomic, readwrite) CGFloat minGraphHeight; 

/*! @property xAxisMargin
	@abstract The x axis margin of the GraphViews contained within the MultiView. 
	@discussion For further discussion of this property, see the documentation for GraphView */
@property (nonatomic, readwrite) CGFloat xAxisMargin;
/*! @property yAxisMargin
	@abstract The y axis margin of the GraphViews contained within the MultiView. 
	@discussion For further discussion of this property, see the documentation for GraphView */
@property (nonatomic, readwrite) CGFloat yAxisMargin;
/*! @property xMin
	@abstract The minimum x value of the GraphViews contained within the MultiView. 
	@discussion In fixed mode, this value is set manually. In scaled mode, it is the smallest x value across
				all of the graphs. */
@property (readwrite) CGFloat xMin;
/*! @property xMax
	@abstract The maximum x value of the GraphViews contained within the MultiView. 
	@discussion In fixed mode, this value is set manually. In scaled mode, it is the largest x value across any of the
				the graphs. */
@property (readwrite) CGFloat xMax;
/*! @property yMin
	@abstract The minimum y value of the GraphViews contained within the MultiView. 
	@discussion In fixed mode, this value is set manually. In scaled mode, each graph has its own minimum value independent of the others,
				so this property is set to -1. */
@property (readwrite) CGFloat yMin;
/*! @property yMax
	@abstract The maximum y value of the GraphViews contained within the MultiView. 
	@discussion In fixed mode, this value is set manually. In scaled mode, each graph has its own maximum value independent of the others,
				so this property is set to -1. */
@property (readwrite) CGFloat yMax;

/*! @group Colors*/
/*! @property containerBackgroundColor
	@abstract The background color of the container view
	@discussion The container view is the top-level view within the scroll view, so this will be drawn beneath any graphs present in the MultiView. */
@property (readwrite, retain) NSColor *containerBackgroundColor;

/*! @property markerLineColor
	@abstract The color of the marker line drawn through the selected x value
	@discussion The line will be drawn vertically across all of the graphs through the selected x value */
@property (readwrite, retain) NSColor *markerLineColor; 

/*! @property majorUnitIndicatorColor
	@abstract The color of the major unit indicator lines
	@discussion The major unit indicator lines are drawn vertically at every major unit. To hide the lines, set this color to [NSColor clearColor]. */
@property (readwrite, retain) NSColor *majorUnitIndicatorColor;

/*! @group Init */
/*! @method initWithFrame:
 @abstract Initializes the GraphMultiView with the provided frame 
 @discussion When the GraphMultiView is initialized using this method, the labels displayed next to each graph are simple text labels 
 @param frame The desired frame for the GraphMiltiView
 @result The GraphMultiView */
- (id)initWithFrame:(NSRect)frameRect;

/*! @method initWithFrame:labelViewControllerClass:
 @abstract Initializes the GraphMultiView with the provided frame and label view controller class
 @discussion The designated initalizer. The labelViewControllerClass must be a subclass of NSViewController that creates and manages the view you wish to use as a label for each 
 graph. A separate instance of this class will be instantiated for each graph that is displayed. The view controller will be created using the init method,
 so it must be able to create its view without any additional parameters (either from a nib or programatically). The view controller will be given the GraphView
 it corresponds to as its representedObject. Please see the class discussion for more information.
 @param frame The desired frame for the GraphMiltiView
 @param labelViewControllerClass The Class object for your custom label view controller class
 @result The GraphMultiView */
- (id)initWithFrame:(NSRect)frame labelViewControllerClass:(Class)labelViewControllerClass;

/*! @method enableAnimations:enable
	@abstract Used to enable animation sequences.  Animations are enabled by default, but should be disabled when a view controller is initialized with data.
	@param enable Whether animations are enabled */
- (void)enableAnimations:(BOOL)enable;

/* Actions */
/*! @group Inserting*/
/*! @method addNewGraphWithDataSource:
	@abstract Adds a new GraphView with the specified data source to the bottom of the GraphMultiView 
	@discussion This method creates a new GraphView with the default settings, inserts it beneath the other graphs
				and returns the newly created graph for further modification. Properties that should be consistent across the graphs in the MultiView, such 
				as the scale, are set automatically and should not be modified when the GraphView is returned. 
	@param dataSource The data source for the new graph
	@result The newly created GraphView */
- (GraphView *)addNewGraphWithDataSource:(id <GraphViewDataSource>)dataSource;
/*! @method insertNewGraphWithDataSource:
	@abstract Inserts a new GraphView with the specified data source at the index specified
	@discussion This method creates a new GraphView with the default settings, inserts it at the specified index
				and returns the newly created graph for further modification. Properties that should be consistent across the graphs in the MultiView, such 
				as the scale, are set automatically and should not be modified when the GraphView is returned. 
	@param dataSource The data source for the new graph
	@param index The index at which the new graph should be inserted
	@result The newly created GraphView */
- (GraphView *)insertNewGraphWithDataSource:(id <GraphViewDataSource>)dataSource atIndex:(NSUInteger)index;
/*! @method addGraph:
	@abstract Adds the supplied GraphView to the bottom of the GraphMultiView
	@discussion This method inserts an existing GraphView beneath the other graphs in the MultiView. Properties that should be the same across all graphs in the MultiView,
				such as the scale, are set to the appropriate values. Other values, such as colors, are not changed.
	@param graphView The graphView to add. Must not be nil. */
- (void)addGraph:(GraphView *)graphView;

/*! @method insertGraph:atIndex:
 @abstract Adds the supplied GraphView at the specified index
 @discussion This method inserts an existing GraphView at the specified index in the MultiView. Properties that should be the same across all graphs in the MultiView,
 such as the scale, are set to the appropriate values. Other values, such as colors, are not changed. If the index is out of bounds, this throws an NSRangeException.
 @param graphView The graphView to add. Must not be nil.
 @param index The index at which the graph should be inserted */
- (void)insertGraph:(GraphView *)graphView atIndex:(NSUInteger)index;

/*! @group Removing*/
/*! @method removeGraphAtIndex:
	@abstract Removes the GraphView at the specified index
	@discussion All GraphViews beneath the removed GraphView are shifted up. If the index is out of bounds, this throws an NSRangeException.
	@param index The index of the GraphView to remove */
- (void)removeGraphAtIndex:(int)index;
/*! @method removeGraph:
	@abstract Removes the specified GraphView
	@discussion All GraphViews beneath the removed GraphView are shifted up. If the GraphView is not in the MultiView, this does nothing. */
- (void)removeGraph:(GraphView *)graphView;

/*! @method moveGraph:toIndex:
	@abstract Moves a graph within the GraphMultiView
	@discussion The destination index should be the current index of the graph above which the specified graphView should be moved. When moving a graph to the bottom, 
				the destination index should be one more than the index of the last graph. Note that the final index of the graph after the move may not be the same as the 
				destination index, as removing the graph from its original position may shift indices down.
	@param graphView The graph being moved
	@param destinationIndex The index to move the graph to */
- (void)moveGraph:(GraphView *)graphView toIndex:(NSUInteger)destinationIndex;

/*! @method moveGraphAtIndex:toIndex:
	@abstract Moves the graph at originIndex to destinationIndex
	@discussion For more information about the meaning of destinationIndex, see the discussion for moveGraph:toIndex:
	@param originIndex The current index of the graph to move
	@param destinationIndex The index to move the graph to */
- (void)moveGraphAtIndex:(NSUInteger)originIndex toIndex:(NSUInteger)destinationIndex;

/*! @group Selecting */

/*! @method selectX:
	@abstract Select the specified x value 
	@discussion The selected x value has a vertical line drawn through it across all of the graphs, and has a marker placed above the scroll view's ruler. 
				If an x value is already selected, it will be deselected before the new x value is selected. The x value will only be selected if it lies within 
				the range currently displayed by the graphs. The GraphMultiView's delegate is notified when an x value is selected. 
	@param xValue The x value to select */

- (void)selectX:(CGFloat)xValue;

/*! @method deselectSelectedX:
	@abstract Deselects the selected x value 
	@discussion If there is no x value selected, this does nothing. The GraphMultiView's delegate is notified when an x value is deselected. */
- (void)deselectSelectedX;

/* Calculators */
/*! @group Sizes*/
/*! @method scaledGraphUnitWidth
	@abstract The number of pixels corresponding to one horizontal unit in the graphs displayed 
	@discussion This value changes when the graph's scale changes. 
	@result The unit width, in pixels */
- (CGFloat)scaledGraphUnitWidth;

/*! @method scaledGraphHeight
	@abstract The height of each GraphView with the GraphMultiView, in pixels. 
	@discussion This value changes when the graph's scale changes. 
	@result The graph height, in pixels. */
- (CGFloat)scaledGraphHeight;

/*! @group Sizing */
/*! @method labelViewSizesNeedUpdating 
	@abstract Tells the GraphMultiView it needs to update the sizes of labels 
	@discussion This method should be called whenever one of the label view's size should change (such as if the title gets longer). If you are using a custom
	NSViewController subclass as your label view controller class, you must implement the idealSize method or the GraphMultiView will not be able to resize the labels.
	See the class discussion for more details. */
- (void)labelViewSizesNeedUpdating;

@end

/*! @group */
/*! @protocol GraphMultiViewDelegate
	@abstract The delegate for the GraphMultiView
	@discussion The delegate responds to various events that occur in the GraphMultiView. */
@protocol GraphMultiViewDelegate <NSObject>
@optional
/*! @method graphMultiView:didSelectX:
	@abstract Called when an x value is selected
	@param graphMultiView The graphMultiView calling this method
	@param xValue The x value that was selected (in real-world coordinates) */
- (void)graphMultiView:(GraphMultiView *)graphMultiView didSelectX:(CGFloat)xValue;
/*! @method graphMultiView:didSelectX:
 @abstract Called when an x value is deselected
 @param graphMultiView The graphMultiView calling this method
 @param xValue The x value that was deselected (in real-world coordinates) */
- (void)graphMultiView:(GraphMultiView *)graphMultiView didDeselectX:(CGFloat)xValue;

/*! @method graphMultiView:didMoveGRaph:toIndex:
	@abstract Called after a graph is moved
	@discussion Note that the destinationIndex may not be the graph's index after the move, as removing the graph from its original location 
				could shift the indices down if the original location is less than the destination. 
	@param graphView The GraphView that was moved
	@param destinationIndex The index the GraphView was moved to*/
- (void)graphMultiView:(GraphMultiView *)graphMultiView didMoveGraph:(GraphView *)graphView toIndex:(NSUInteger)destinationIndex;

@end
