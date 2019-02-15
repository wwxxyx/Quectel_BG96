//
//  DetailsViewLeaf.h
//  ATSMacApp
//
//  Created by Joe Basu on 7/2/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

/*!
 @header     DetailsViewLeaf
 @discussion This header file describes the interface for the object that 
             represents a leaf node in the event details view tree structure. It
             also defines a delegate protocol.
 */

#import <Foundation/Foundation.h>
#import <WebKit/WebKit.h>

@class DetailsViewSection;
@class DetailsViewLeaf;

/*!
 @const      kDetailsViewLeafItemClickedNotificationName
 @abstract   The name of the notification posted when the user clicks on a 
             details view leaf item.
 */
extern NSString *kDetailsViewLeafItemClickedNotificationName;

/*!
 @const      kDetailsViewLeafItemMouseOverNotificationName
 @abstract   The name of the notification posted when the user mouses over a 
             details view leaf item.
 */
extern NSString *kDetailsViewLeafItemMouseOverNotificationName;

/*!
 @const      kDetailsViewLeafItemMouseOffNotificationName
 @abstract   The name of the notification posted when the user mouses off a 
             details view leaf item.
 */
extern NSString *kDetailsViewLeafItemMouseOffNotificationName;

/*!
 @protocol   DetailsViewLeafDelegate
 @abstract   Delegate protocol for the DetailsViewLeaf class. Delegate objects 
             are notified of important events in the leaf item lifecycle, such 
             as when the contents of the item are ready to be displayed.
 */
@protocol DetailsViewLeafDelegate <NSObject>

@required

/*!
 @method     detailsViewLeafIsReady:
 @abstract   Sent when the details view leaf is ready to be displayed.
 @discussion This method is required.
 */
- (void)detailsViewLeafIsReady:(DetailsViewLeaf *)leaf;

/*!
 @method        detailsViewLeafDidResize:
 @abstract      Sent when the defailts view leaf web view content did resize
 @discussion    This method is required
 */
- (void)detailsViewLeafDidResize:(DetailsViewLeaf *)leaf;

/*!
 @method        detailsViewLeaf:linkClicked:
 @abstract      Called when a link is clicked within the leaf.
 @param         leaf
                The details view leaf in which the link click originated.
 @param         destination
                The destination URL, path, or keyword to which the link points.
 */
- (void)detailsViewLeaf:(DetailsViewLeaf *)leaf linkClicked:(NSString *)destination;

@end

/*!
 @class      DetailsViewLeaf
 @abstract   DetailsViewLeaf instances represent leaf nodes in the details view 
             tree structure. They are responsible for rendering their contents 
             for display in a trace window.
 */
@interface DetailsViewLeaf : NSObject

/*!
 @property   delegate
 @abstract   A delegate object. Delegates are notified of important events in 
             the leaf view item's lifecycle, such as when the contents of the 
             item are ready to be displayed.
 */
@property (nonatomic, assign) id<DetailsViewLeafDelegate> delegate;

/*!
 @property   parentDetailsViewSection
 @abstract   The details view section that contains the leaf.
 */
@property (nonatomic, assign) DetailsViewSection *parentDetailsViewSection;

/*!
 @property   webView
 @abstract   The web view that renders and displays the contents of the leaf.
 */
@property (nonatomic, readonly, retain) WebView *webView;

/*!
 @property   rowHeight
 @abstract   The height of the leaf's rendered content.
 */
@property (nonatomic, readonly) CGFloat rowHeight;

/*!
 @property   didFrameLoad
 @abstract   A flag indicating whether or not the leaf's contents have been 
             rendered and are ready to be displayed.
 */
@property (nonatomic, assign) BOOL didFrameLoad;

/*!
 @method     renderWithLeafDelegate:
 @abstract   Sets the leaf's delegate and begins the process of rendering the 
             leaf's contents for display.
 @param      delegate
             The object to set as delegate and notify when the rendering has 
             completed.
 */
- (void)renderWithLeafDelegate:(id<DetailsViewLeafDelegate>)delegate;

/*!
 @method     makeID:clickableToRange:
 @abstract   Registers the id to create a clickable range notification.
 @param      elementID
             The ID of the DOM element that should be clickable.
 @param      range
             The range of bytes in the message payload that corresponds to the
             clickable DOM element.
 */
- (void)makeID:(NSString *)elementID clickableToRange:(NSRange)range;

/*!
 @method    teardown
 @abstract  Prepares the leaf for removal from view and deallocation.
 */
- (void)teardown;

@end
