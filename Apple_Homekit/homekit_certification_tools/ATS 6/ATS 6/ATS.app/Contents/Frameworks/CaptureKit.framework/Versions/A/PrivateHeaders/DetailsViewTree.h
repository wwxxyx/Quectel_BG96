//
//  DetailsViewTree.h
//  ATS
//
//  Created by Joel Levin on 8/31/09.
//  Copyright 2009 Apple Inc.. All rights reserved.
//

/*!
 @header     DetailsViewTree
 @discussion This header file describes the interface for the tree structure
             that manages the display of sections in the details sidebar of a
             trace window. It also defines a delegate protocol.
 */

#import <Foundation/Foundation.h>
#import <CaptureKit/DetailsViewSection.h>
#import <CaptureKit/DetailsViewTreeDelegate.h>

/*!
 @class      DetailsViewTree
 @abstract   DetailsViewTree manages the collection of objects that display the 
             individual sections within the details sidebar of a trace window.
 */
@interface DetailsViewTree : NSObject <DetailsViewSectionDelegate>

/*!
 @property   delegate
 @abstract   The delegate object that wishes to be notified of important events 
             in the tree's lifecycle.
 */
@property (nonatomic, assign) id<DetailsViewTreeDelegate> delegate;

/*!
 @property   ready
 @abstract   A flag indicating whether or not the tree is ready to be displayed.
 */
@property (nonatomic, readonly, getter=isReady) BOOL ready;

/*!
 @method     addSectionNode:
 @abstract   Adds a new top-level section to the tree.
 @param      section
             The new top-level section to add to the tree.
 */
- (void)addSectionNode:(DetailsViewSection *)section;

/*!
 @method     sectionAtIndex:
 @abstract   Returns the details view section at the given index.
 @param      sectionIndex
             The index of the section to return.
 @return     The details view section at the given index.
 */
- (DetailsViewSection *)sectionAtIndex:(NSUInteger)sectionIndex;

/*!
 @method     sectionCount
 @abstract   Returns the number of top-level sections in the tree.
 @return     The number of top-level sections in the tree.
 */
- (NSUInteger)sectionCount;

/*!
 @method     assignTreeReferences
 @abstract   Finalizes the tree structure, ensuring that the tree is ready for 
             display.
 */
- (void)assignTreeReferences;

/*!
 @method     renderWithTreeDelegate:
 @abstract   Begins the process of rendering the tree's sections for display.
 @param      delegate
             The delegate object that should be called when the tree is ready 
             for display.
 */
- (void)renderWithTreeDelegate:(id<DetailsViewTreeDelegate>)delegate;

/*!
 @method     teardown
 @abstract   Prepares the tree structure for deallocation.
 */
- (void)teardown;

@end
