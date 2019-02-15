//
//  GraphView.h
//  MacGraphin
//
//  Created by David Silver on 6/22/10.
//  Copyright 2010 Apple, Inc. All rights reserved.
//

/*! @header GraphView 
 @abstract GraphView class and associated protocols */

#import <Cocoa/Cocoa.h>

@class GraphManager;
@protocol GraphViewDataSource;
@protocol GraphViewDelegate;

/*! @class GraphView
	@abstract Displays a line graph of a set of data points 
	@discussion A GraphView draws a line graph of a set of points supplied by its data source. Lines are drawn between points, and small circles drawn at the points
				themselves. Attributes such as colors and line width are configurable through properties. Optionally, the GraphView can draw indicator lines at mutiples
				of the unit value, although this behavior is disabled by default. 
 
				The GraphView has two different display modes. In scaled mode, the default, the user specifies the graph's horizontal scale, which represens the number of
				pixels per unit. The minimum value supplied by the data source is drawn at the start of the graph, and the remainder of the points are drawn based
				on the set scale. If the GraphView's frame is wider than is necessary to display all of the points, the extra portion of the graph will be blank. It is the 
				user's responsibility to ensure the graph's frame is at least as wide as necessary to display all of the points at the current scale. The vertical scale is 
				calculated automatically so that the minimum y value specified by the data source is at the bottom of the GraphView, and the maximum y value specified by the data source
				is at the top of the GraphView. Changing the vertical scale should be done by changing the frame of the GraphView. When in scaled mode, the xAxisScale property must
				be set manually. 
 
				Scaled mode is active when the hasFixedExtrema property is set to NO. When in scaled mode, setting the hasFixedXMin property to YES allows the minimum x value to be set by the user
				instead of being obtained from the data source. This allows for scaled mode behavior but with a fixed minimum x value, which is useful if the horizontal coordinates of multiple graph
				views need to be aligned. When the hasFixedXMin property is set to YES, the xMin property must be set manually. 
 
				Fixed mode can be activated by setting the hasFixedExtrema property to YES. In this mode, the minimum and maximum x and y values displayed are set by the user, creating a "region" 
				of the graph that is displayed. The scale in both directions is calculated so that this region fills the entire frame of the GraphView. Points outside the region are not visible, 
				but lines to these points will drawn going off the screen, to indicate that there is additional data not visible. When in fixed mode, the xMin, xMax, yMin, and yMax properties must
				be set manually. 
 
				The GraphView uses two different types of points. "Graph Points" are points in the real-world units represented by the graph. These are the points that would
				consider the coordinates were the graph to be drawn by hand. For example, in a graph of volts over time, a reading of 3 volts at 2 seconds would be represented
				by the graph point (2,3). "View Points" are points within the view's coordinate system, and thus represent the actual coordinates at which the graph points are drawn.
				The view point corresponding to a graph point- and thus the point's position within the view - varies depending on the GraphView's current mimimum value, maximum value,
				scale and margins. The GraphView will automatically convert graph points to the corresponding view point when drawing, so users of this class will rarely need to deal with 
				view points at all. All points supplied to the GraphView through the data source or property methods should be graph points. 
 */
@interface GraphView : NSView {
	id <GraphViewDataSource> _dataSource;
	id <GraphViewDelegate> _delegate;
	
	NSString *_title;
	
	/* Reloading */
	BOOL _disableDataReloading;
	
	/* Colors */
	NSColor *_backgroundColor; 
	NSColor *_lineColor; 
	NSColor *_pointColor;
	NSColor *_unitIndicatorLineColor;
	
	/* Appearance */
	CGFloat _xAxisMargin; //pixels
	CGFloat _yAxisMargin; //pixels
	CGFloat _backgroundRectRadius; 
	CGFloat _dataLineWidth; //in Quartz user space units
	CGFloat _unitIndicatorLineWidth; //in Quartz user space units
	
	// With what frequency should the major units have lines through them?
	// Set to zero for no lines
	CGFloat _xUnitIndicatorLineFrequency;
	CGFloat _yUnitIndicatorLineFrequency;
	
	/** Drawing size options **/
	/* Drawing parameters */
	BOOL _hasFixedExtrema; //Switches between scaled mode and fixed mode
	BOOL _hasFixedXMin; //Should the minimum x value be fixed?
	BOOL _hasFixedYMax; //Should the maximum y value be fixed?

	/* Scaled mode - fixedExtrema is false
	 * Must set these values manually */
	CGFloat _xScaleBase; //base number of pixels per one unit
	CGFloat _xAxisScale; //multiples of the default
	CGFloat _yScaleBase; // y-axis scale is always computed automatically, and thus cannot be 
	CGFloat _yAxisScale; // set automatically
	
	/* Fixed mode - fixedExtrema is true
	 * Must set these values manually */
	CGFloat _xMax;
	CGFloat _yMin;
	CGFloat _yMax;
	
	/* If either fixedXMin or fixedExtrema is true, must set this value
	 * Otherwise, the value is set automatically */
	CGFloat _xMin;
	
}
/***** Properties *****/

/*! @property dataSource
	@abstract The data source for the GraphView 
	@discussion The data source supplies the graph view with the points to draw, according to the GraphViewDataSource protocol. 
 				When this variables is set, the new data source will be sent the becomeDataSourceForGraph: message to allow for setup.
 				If the points change after the data source is first set, the GraphView should be passed the reloadData: method. */
@property (nonatomic, readwrite, assign) id <GraphViewDataSource> dataSource; 
/*! @property delegate
	@abstract The delegate for the GraphView
 	@discussion The delegate must implement the GraphViewDelegate protocol. */
@property (readwrite, assign) id <GraphViewDelegate> delegate;

/*! @property title
 @abstract The title of this GraphView
 @discussion  The GraphView itself does nothing with this value, but it may be used elsewhere. */
@property (readwrite, retain) NSString *title;

/*! @property disableDataReloading
	@abstract Prevents the GraphView from reloading its data 
	@discussion This will prevent the data source from reloading its data when reloadData: is called (generally by its data source). If the GraphView redraws
				for other reasons while data reloading is disabled (for example, if the GraphView is scrolled off screen and then scrolled back), any new values 
				supplied by the data source within the redrawn region will still appear. 
 
				When data reloading is reenabled, the GraphView will automatically reload its data. */
@property (nonatomic, readwrite) BOOL disableDataReloading;

/*! @group Display Settings*/
/*! @property backgroundColor
 	@abstract The background color of the GraphView
	@discussion The background color will fill the entire bounds of the graph, potentially excepting the corners if they are rounded off. */
@property (nonatomic, readwrite, retain) NSColor *backgroundColor;
/*! @property lineColor
	@abstract The color of the lines drawn between data points. 
	@discussion This property only affects the color of the lines themselves. The color of the points is set by the pointColor property, and need not
 				be the same as the line color. */
@property (nonatomic, readwrite, retain) NSColor *lineColor;
/*! @property pointColor
	@abstract The color of the dots drawn at each data point 
    @discussion This property only affects the color of the points themselves. The color of the lines is set by the lineColor property, and need not be
 				the same as the pointColor. If you do not want dots to be drawn at the data points, this should be set to [NSColor clearColor] */
@property (nonatomic, readwrite, retain) NSColor *pointColor;

/*! @property xAxisMargin
	@abstract The size of the left and right margins
	@discussion The margin is the distance from the left and right edges of the view's bounds to the first and last data point. The margin 
 				should be specified in pixels. This property sets both the left and right margins. 
				
				If the GraphView is in fixed mode and there are points beyond the min and max values the graph is displaying, lines to those points
				may be visible within the margin region. */
@property (nonatomic, readwrite) CGFloat xAxisMargin;
/*! @property yAxisMargin
	@abstract The size of the top and bottom margins 
	@discussion The margin is the distance from the top and bottom edges of the view's bounds to the bottommost and topmost data points. The 
 				margin should be specified in pixels. This property sets both the top and bottom margins
 
				If the GraphView is in fixed mode and there are points beyond the min and max values the graph is displaying, lines to those points
				may be visible within the margin region. */
@property (nonatomic, readwrite) CGFloat yAxisMargin;
/*! @property backgroundRectRadius 
	@abstract The radius of the rounded rectangle drawn as the view's background
	@discussion The radius affects how rounded the corners of the rectangle are, with a larger radius resulting in a more rounded corner. The rectangle 
 				clips the view, so when setting this value, ensure the margins are large enough that no points will be drawn within the clipped region. */
@property (nonatomic, readwrite) CGFloat backgroundRectRadius;
/*! @property dataLineWidth
	@abstract The width of the lines connecting data points
	@discussion The line width is in Quartz userspace units, and has a default value of 1 */
@property (readwrite) CGFloat dataLineWidth;

/*! @group Unit Indicator Lines*/
/*! @property unitIndicatorLineWidth
    @abstract The width of the unit indicator lines
    @discussion  The line width is in Quartz user space units, and has a default value of 1. */

@property (readwrite) CGFloat unitIndicatorLineWidth;

/*! @property xUnitIndicatorLineFrequency
    @abstract The frequency of the unit indicator lines across the x axis
    @discussion  The first unit indicator line is drawn across the minimum value, and subsequent unit indicator lines are drawn across multiples of the unit width
 				 beyond the minimum, with a frequency determined by this property. For example, a frequency of 1 means that a line will be drawn every one unit
 				 width beyond the minimum, and a frequency of 2 means that a line will be drawn every two unit widths beyond that. Lines are drawn vertically
 				 across the entire graph.
 
 				 Set this property to zero to draw not unit indicator lines at all, which is default. */
@property (readwrite) CGFloat xUnitIndicatorLineFrequency;
/*! @property yUnitIndicatorLineFrequency
	@abstract The frequency of the unit indicator lines across the y axis
	@discussion  The first unit indicator line is drawn across the minimum value, and subsequent unit indicator lines are drawn across multiples of the unit width
 				 beyond the minimum, with a frequency determined by this property. For example, a frequency of 1 means that a line will be drawn every one unit
 				 width beyond the minimum, and a frequency of 2 means that a line will be drawn every two unit widths beyond that. Lines are drawn horizontally
 				 across the entire graph.
 
 				 Set this property to zero to not draw unit indicator lines at all, which is default. */

@property (readwrite) CGFloat yUnitIndicatorLineFrequency;

/*! @property unitIndicatorLineColor
 @abstract The color of the unit indicator lines
 @discussion This affects both the horizontal and vertical unit indicator lines. */
@property (readwrite, retain) NSColor *unitIndicatorLineColor;

/*! @group Mode Switching*/
/*! @property hasFixedExtrema
	@abstract Specifies whether or not the min and max values an the graph should be fixed.
	@discussion Setting this to YES activates Fixed Mode, in which the minimum and maximum x and y values are fixed. The scale for each axis will be calculated 
 				automatically such that the specified region will take up the entire bounds of the graph (minus the margins). If there are points outside the 
 				visible portion of the graph, lines will be drawn to them through the margins to the edge of the view, providing a visual indicator that there are
 				points outside the visible range. The default value is NO. 
 		
 				If this property is set to YES, then the xMin, xMax, yMin, and yMax variables must all be set manually. For further discussion of the GraphView's modes,
 				see the class's discussion. */

@property (readwrite) BOOL hasFixedExtrema;
/*! @property hasFixedXMin
 @abstract Specifies whether or not the minimum x value is fixed
 @discussion This property allows the graph to be in scaled mode but still have a fixed minimum x value. The minium x value and the scales of both axes are set manually.
 The maximum x value and both the minimum and maximum y values are be obtained from the data source. The default value is NO.
 
 If in Fixed Mode (hasFixedExtrema is YES), this property accomplishes nothing. If this property is set to YES, and Fixed Mode is not activated, then the
 xMin, xAxisScale, and yAxisScale variables must be set manually. For further discussion of the GraphView's modes, see the class's discussion. */
@property (readwrite) BOOL hasFixedXMin;

/*! @group Scale*/
/*! @property xAxisScale
 @abstract The scale of the x axis
 @discussion The scale controls how many pixels correspond to one unit. This property provides a multiplier to the base scale value to increase or decrease the graph's scale.
 This value, and not xScaleBase, should be used to dynamically zoom the graph in or out. The default value is 1. 
 
 This value should only be set manually in Scaled Mode. In Fixed Mode, the scale is calculated automatically such that the specified region fills the graph's entire 
 bounds. For further discussion of the GraphView's modes, see the class's discussion. */
@property (nonatomic, readwrite) CGFloat xAxisScale;
/*! @property xScaleBase
 	@abstract The base value of the x axis's scale
 	@discussion Together with the xAxisScale property, this property determines the actual scale of the x axis. The xAxisScale property provides a multiplier to this property's value 
 				that together are used to calculate the number of pixels corresponding to one unit. The default base value is sufficient for most purposes, and so you will normally
 				have no need to interact with this property directly. To increase or decrease the scale, you will normally just increase or decrease the xAxis scale variable instead. */
@property (nonatomic, readwrite) CGFloat xScaleBase;

/*! @group Min/Max*/
/*! @property xMin
 	@abstract The minimum x value displayed on the graph
	@discussion In Scaled Mode, this value is the same as the minimum value specified by the data source. If either hasFixedExtrema or hasFixedXMin is to YES, however,
 				this value must be set manually. For further discussion of the GraphView's modes, see the class's discussion.*/
@property (nonatomic, readwrite) CGFloat xMin;
/*! @property xMax
	@abstract The maximum x value displayed on the graph
	@discussion In Scaled Mode, this value is the same as the maximum value specified by the data source. In Fixed Mode, this value must be set manually. 
 				For further discussion of the GraphView's modes, see the class's discussion. */
@property (nonatomic, readwrite) CGFloat xMax;
/*! @property yMin
	@abstract The minimum y value displayed on the graph
	@discussion In Scaled Mode, this value is the same as the minimum value specified by the data source. In Fixed Mode, this value must be set manually. 
				For further discussion of the GraphView's modes, see the class's discussion. */
@property (nonatomic, readwrite) CGFloat yMin;
/*! @property yMax
	@abstract The maximum x value displayed on the graph
	@discussion In Scaled Mode, this value is the same as the maximum value specified by the data source. In Fixed Mode, this value must be set manually. 
 				For further discussion of the GraphView's modes, see the class's discussion. */
@property (nonatomic, readwrite) CGFloat yMax;

/* Methods */
/* @group Calculators */
/*! @method coordinateForXValue:inGraphWithUnitWidth:xMargin: 
	@abstract Returns the coordinate of the specified x value in a graph with the provided unit width and margin 
	@param xValue The x value you want to convert (in real world coordinates)
	@param unitWidth The unit width of the graph
	@param xMargin The x-axis margin of the graph
	@result The coordinate in the graph's coordinate system of the specified x value */
+ (CGFloat)coordinateForXValue:(float)xValue inGraphWithUnitWidth:(float)unitWidth xMargin:(float)xMargin;
/*! @method xValueForCoordinate:inGraphWithUnitWidth:xMargin 
	@abstract Returns the x value in real-world units of the point with the specified coordinate in a graph with the specified unit width and margins
	@param coordinate The coordinate you want to convert (in the graph's coordinate system)
	@param unitWidth The unit width of the graph
	@param xMargin The x-axis margin of the graph
	@result The x value (in real world units) corresponding to the specified coordinate */
+ (CGFloat)xValueForCoordinate:(float)coordinate inGraphWithUnitWidth:(float)unitWidth xMargin:(float)xMargin;

/*! @method widthOfGraphWithMinValue:maxValue:unitWidth:margin: 
	@abstract Returns the width necessary for a graph with the specified parameters to display all of its points
	@discussion It is possible for such a graph to actually be larger than this value. The remaining space would simply be empty. 
	@param minValue The minimum value of the graph
	@param maxValue The maximum value of the graph
	@param unitWidth The number of pixels corresponding to one unit in the graph 
	@param margin The x-axis margin of the graph
	@result The width of the graph */
+ (CGFloat)widthOfGraphWithMinValue:(CGFloat)minValue maxValue:(CGFloat)maxValue unitWidth:(CGFloat)unitWidth margin:(CGFloat)margin;

/*! @method unitWidth 
	@abstract The number of pixels corresponding to one horizontal unit in the graph 
	@result The unit width */
- (CGFloat)unitWidth; 

/*! @method unitHeight 
 @abstract The number of pixels corresponding to one vertical unit in the graph 
 @result The unit height */
- (CGFloat)unitHeight;

/*! @group Actions*/
/*! @method reloadData
	@abstract Reloads the data displayed in the GraphView
	@discussion This should be called whenever the data held by the data source changes. */
- (void)reloadData;

/*! @method backgroundGradient
	@abstract The gradient drawn in the GraphView's background 
	@discussion This returns the gradient object used to draw the GraphView's background. It is automatically calculated from the background color.
				It may be useful if you want to give another view the same color scheme as the GraphView. */
- (NSGradient *)backgroundGradient;

/*! @group Point Conversion*/
/*! @method viewPointFromGraphPoint:
	@abstract Converts a point from "real-world" units to the corresponding point within the view's coordinate system
	@discussion The graph point should be in the real-world units the graph represents, such as (seconds, volts). The view point returned
				indicates the position within the view's coordinate system at which the graph point will be drawn, which varies depending on the
				graph's scale, minimum value, maximum value, and margins.
	@param graphPoint A point in real-world units
	@result The corresponding point in the view's coordinate system */
- (NSPoint)viewPointFromGraphPoint:(NSPoint)graphPoint;
/*! @method graphPointFromViewPoint:
	@abstract Converts a point within the view's coordinate system to the corresponding point in "real-world" units
	@discussion The returned point is the point in real-world units corresponding to the supplied view point, which varies depending on the graph's 
				scale, minimum value, maximum value, and margins. 
	@param viewPoint A point in the view's coordinate system
	@result The corresponding point in the real-world units of the graph */
- (NSPoint)graphPointFromViewPoint:(NSPoint)viewPoint;

@end

/*! @group */
/* Protocols */

/*! @protocol GraphViewDataSource
 *	@abstract Defines the data source methods for the GraphView 
 *	@discussion The data source is responsible for suppyling the points through which the graph will draw lines. Points should be in the "real-world"
 *				units of the data, and not in the coordinates system of the GraphView itself. */
@protocol GraphViewDataSource <NSObject>
/*! @method minXForGraph:
	@abstract The minimum x value in the data set
	@discussion This value is treated as the minimum x value in the data set when calculating coordinates and scale. Note that it is not absolutely necessary that this
				be the <i>actual</i> minimum x value, as in some situtations you may want to start the graph at a value lower than the actual minimum data point (for 
				example, in a graph over time, if the lowest reading is at t = .5, you may want to start the graph itself at t = 0).

				This value may or may not be the minimum x value displayed, depending on the mode the GraphView is in. For further discussion of the GraphView's modes, 
				see the class's discussion.
	 @param graphView The graphView calling this method
	 @result The minimum x value (in real-world units)*/
- (CGFloat)minXForGraph:(GraphView *)graphView;
/*! @method maxXForGraph:
	@abstract The minimum x value in the data set
	@discussion This value is treated as the maximum x value in the data set when calculating coordinates and scale. Note that it is not absolutely necessary that this
				be the <i>actual</i> maximum y value. 

				This value may or may not be the maximum x value displayed, depending on the mode the GraphView is in. For further discussion of the GraphView's modes, 
				see the class's discussion.
	 @param graphView The graphView calling this method
	 @result The maximum x value (in real-world units)*/
- (CGFloat)maxXForGraph:(GraphView *)graphView;
/*! @method minYForGraph
 	@abstract The minimum y value in the data set
 	@discussion This value is treated as the minimum y value in the data set when calculating coordinates and scale. Note that it is not absolutely necessary that this
 				be the <i>actual</i> minimum y value. 
 
 				This value may or may not be the maximum x value displayed, depending on the mode the GraphView is in. For further discussion of the GraphView's modes, 
 				see the class's discussion.
	@param graphView The graphView calling this method
	@result The minimum y value (in real-world units)*/
- (CGFloat)minYForGraph:(GraphView *)graphView;
/*! @method maxYForGraph
	@abstract The maximum y value in the data set
	@discussion This value is treated as the maximum y value in the data set when calculating coordinates and scale. Note that it is not absolutely necessary that this
				be the <i>actual</i> maximum y value.

				This value may or may not be the maximum y value displayed, depending on the mode the GraphView is in. For further discussion of the GraphView's modes, 
				see the class's discussion.
	@param graphView The graphView calling this method
	@result The maximum y value (in real-world units) */
- (CGFloat)maxYForGraph:(GraphView *)graphView;


/*! @method pointsForGraph:surroundingX:toX:
	@abstract The points to be displayed surrounding the specified x values
	@discussion This method is used by the GraphView to obtain the points it needs to draw. The float parameters indicate the minimum and maximum x coordinates of the area
				being redrawn. In order to ensure that lines entering and exiting this region are drawn at the correct angle, this method should return not only the points 
				within the specified region, but also the point immediately preceeding the region's first point and immediately succeeding the region's last point (if such 
				points exist). 
 
				The objects in the returned array should be NSValues wrapping NSPoints
	@param graphView The graph view calling this method
	@param startX The minimum x value (in real-world units) in the region being redrawn
	@param endX The maximum x value (in real-world units) in the region being redrawn
	@result An NSArray of NSValue objects wrapping NSPoints*/
- (NSArray *)pointsForGraph:(GraphView *)graphView surroundingX:(CGFloat)startX toX:(CGFloat)endX;

@optional
/*! @method becomeDataSourceForGraph:
 @abstract Notifies the data source that it has become the data source for the supplied graph view 
 @discussion This method is called when the dataSource property of the GraphView is set and can be used for any necessary setup of the data source object.
 If the data source supplies data over time, this method call also indicates that the GraphView is ready to start receiving data. 
 @param graphView The graph view for which the receiver just became the data source */
- (void)becomeDataSourceForGraph:(GraphView *)graphView; 

/*! @method dotLocationsForGraph:surroundingX:toX:
 @abstract The points at which to draw do ts in the GraphView
 @discussion Define this method if you want to draw dots at locations different from the points that serve as end points to the graph's lines (which are specified in the pointsForGraph:surroundX:toX method). 
			 The points given by this method need not be the same as the points supplied in the pointsForGraph:surroundX:toX method - this way, you can draw dots on only a subset of the line endpoints, or
			 at locations not connected to lines. If this method is not defined, dots will be drawn at all of the points specified by the pointsForGraph:surroundingX:toX method. 
			
			The objects in the returned array should be NSValues wrapping NSPoints
 @param graphView The graph view calling this method
 @param startX The minimum x value (in real-world units) in the region being redrawn
 @param endX The maximum x value (in real-world units) in the region being redrawn
 @result An NSArray of NSValue objects wrapping NSPoints in real-world units*/
- (NSArray *)dotLocationsForGraph:(GraphView *)graphView surroundingX:(CGFloat)startX toX:(CGFloat)endX;

/*! @method providesTheoreticalYExtremaForGraph: 
 @abstract Is the data source be asked for theoretical extrema? 
 @discussion This method exists so that a data source may implement the theoreticalMinYForGraph: and theoreticalMaxYForGraph: methods without necessarily 
 always providing theoretical values (in case the data source class serves as the data source for multiple graphs) 
 @param graphView The GraphView calling this method 
 @result A boolean indicate whether or not the data source should be asked for theoretical extrema */
- (BOOL)providesTheoreticalYExtremaForGraph:(GraphView *)graphView; 

/*! @method providesAbsoluteYExtremaForGraph: 
 @abstract Is the data source be asked for absolute extrema? 
 @discussion This method exists so that a data source may implement the absoluteMaxYForGraph: methods without necessarily 
 always providing aboslute values (in case the data source class serves as the data source for multiple graphs) 
 @param graphView The GraphView calling this method 
 @result A boolean indicating whether or not the data source should be asked for absolute extrema */
- (BOOL)providesAbsoluteYExtremaForGraph:(GraphView *)graphView; 

/*! @method theoreticalMinYForGraph:
 *  @abstract The theoretical minimum y value for the specified graph
	@discussion The theoretical minimum overrides the actual minimum (reported by the minYForGraph: method) when calculating the graph's scale. If the graph's data always falls within a range of values
				and you want the graph to always display the full range even if there are presently no points at the extremes, then you can set the theoretical minimum and maximum to force the graph to 
				display the full range. If the actual minimum is less than the theoretical minimum, the theoretical minimum will be ignored.
	@param graphView The graphView calling this method 
	@result The theoretical minimum y  value for the graph */
- (CGFloat)theoreticalMinYForGraph:(GraphView *)graphView;

/*! @method theoreticalMaxYForGraph:
 *  @abstract The theoretical maximum y value for the specified graph
 @discussion The theoretical maximum overrides the actual maximum (reported by the maxYForGraph: method) when calculating the graph's scale. If the graph's data always falls within a range of values
 and you want the graph to always display the full range even if there are presently no points at the extremes, then you can set the theoretical minimum and maximum to force the graph to 
 display the full range. If the actual maximum is greater than the theoretical maximum, the theoretical maximum will be ignored.
 @param graphView The graphView calling this method 
 @result The theoretical maximum y  value for the graph */
- (CGFloat)theoreticalMaxYForGraph:(GraphView *)graphView;

/*! @method absoluteMaxYForGraph:
 *  @abstract The absolute maximum y value for the specified graph
 @discussion The absolute maximum overrides the actual maximum (reported by the maxYForGraph: method) when calculating the graph's scale. If the graph's data always falls outside of a range of values
 and you want the graph not to  display the full range even if there are presently points at the extremes, then you can set the absolute maximum to force the graph to 
 display the a limited range. If the actual maximum is greater than the absolute maximum, the actual maximum will be ignored.
 @param graphView The graphView calling this method 
 @result The absolute maximum y  value for the graph */
- (CGFloat)absoluteMaxYForGraph:(GraphView *)graphView;

@end

/*! @protocol GraphViewDelegate
	@abstract Defines the delegate methods for the GraphView
	@discussion The delegate is responsible for responding to events that occur within the GraphView */
@protocol GraphViewDelegate <NSObject> 

/*! @method graphViewDidReloadData:
	@abstract Called whenever the graph view reloads its data 
	@discussion This method is called whenever the data within the graph view is reloaded, which indicates a change in the data points or in the minimum or maximum value
				in the data set. 
	@param graphView The graph view calling this method */
- (void)graphViewDidReloadData:(GraphView *)graphView;
@end
