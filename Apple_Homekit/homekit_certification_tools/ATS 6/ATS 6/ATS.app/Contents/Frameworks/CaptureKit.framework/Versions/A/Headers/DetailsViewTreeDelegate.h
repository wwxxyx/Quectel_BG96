//
//  DetailsViewTreeDelegate.h
//  ATSMacApp
//
//  Created by Wesley McCloy on 9/5/17.
//  Copyright © 2017 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class DetailsViewTree;

/*!
 @protocol   DetailsViewTreeDelegate
 @abstract   Delegate protocol for the DetailsViewTree class. Delegates are
 notified of important events in the tree's lifecycle, such as when
 the tree is ready to be displayed.
 */
@protocol DetailsViewTreeDelegate <NSObject>

@required

/*!
 @method     detailsViewTreeIsReady:
 @abstract   Sent when the tree's subviews have finished rendering and the tree
 is ready for display.
 @param      tree
 The details view tree that is ready for display.
 @discussion This method is required.
 */
- (void)detailsViewTreeIsReady:(DetailsViewTree *)tree;

/*!
 @method        detailsViewItemDidResize:
 @abstract      Called when an item resizes
 @param         item
 The details view item
 @discussion    This method is required
 */
- (void)detailsViewItemDidResize:(id)item;

/*!
 @method        detailsViewTree:linkClicked:
 @abstract      Called when a link is clicked within one of the detail view's
 leaves.
 @param         tree
 The details view tree in which the link click originated.
 @param         destination
 The destination URL, path, or keyword to which the link points.
 */
- (void)detailsViewTree:(DetailsViewTree *)tree linkClicked:(NSString *)destination;

@end
