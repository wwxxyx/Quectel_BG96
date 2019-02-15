//
//  DetailsViewSection.h
//  ATS
//
//  Created by Joel Levin on 8/31/09.
//  Copyright 2009 Apple Inc.. All rights reserved.
//

/*!
 @header     DetailsViewSection
 @discussion This header file describes the interface for the class that 
             represents a section within the details sidebar in a trace window. 
             It also defines a delegate protocol.
 */

#import <Foundation/Foundation.h>
#import <CaptureKit/DetailsViewLeaf.h>

@class DetailsViewTree;
@class DetailsViewSection;

/*!
 @protocol   DetailsViewSectionDelegate
 @abstract   Delegate protocol for the DetailsViewSection class. Delegates are 
             notified of important events in the section's lifecycle, such as 
             when the section is ready for display.
 */
@protocol DetailsViewSectionDelegate <NSObject>

@required

/*!
 @method     detailsViewSectionIsReady:
 @abstract   Called when the section is ready for display.
 @param      section
             The section that is ready for display.
 @discussion This method is required.
 */
- (void)detailsViewSectionIsReady:(DetailsViewSection *)section;

/*!
 @method        detailsViewItemDidResize:
 @abstract      Called when an item resizes
 @param         item
                The details view item
 @discussion    This method is required
 */
- (void)detailsViewItemDidResize:(id)item;

/*!
 @method        detailsViewSection:linkClicked:
 @abstract      Called when a link is clicked within one of the detail view
                section's leaves.
 @param         section
                The details view section in which the link click originated.
 @param         destination
                The destination URL, path, or keyword to which the link points.
 */
- (void)detailsViewSection:(DetailsViewSection *)section linkClicked:(NSString *)destination;

@end

/*!
 @class      DetailsViewSection
 @abstract   DetailsViewSection is an abstract class that manages the contents
             of a section within the details view sidebar in a trace window.
 */
@interface DetailsViewSection : NSObject <DetailsViewLeafDelegate>

/*!
 @property   delegate
 @abstract   The delegate object that wishes to be notified of important events
             in the tree's lifecycle.
 */
@property (nonatomic, assign) id<DetailsViewSectionDelegate> delegate;

/*!
 @property   tree
 @abstract   The details view tree structure containing the section.
 */
@property (nonatomic, assign) DetailsViewTree *tree;

/*!
 @property   title
 @abstract   The section's title.
 */
@property (nonatomic, readonly, copy) NSString *title;

/*!
 @property   identifier
 @abstract   The section's identifier.
 */
@property (nonatomic, readonly, copy) NSString *identifier;

/*!
 @property   detailsViewLeaf
 @abstract   The leaf node underneath the section. Leaf nodes are ultimately 
             responsible for rendering the contents of the section.
 */
@property (nonatomic, readonly, retain) DetailsViewLeaf *detailsViewLeaf;

/*!
 @property   lineCount
 @abstract   The number of lines of text in the contents of the section.
 */
@property (nonatomic) NSUInteger lineCount;

/*!
 @property   startsDisclosed
 @abstract   A flag indicating whether or not the section should be expanded 
             initially.
 */
@property (nonatomic) BOOL startsDisclosed;

/*!
 @property   ready
 @abstract   A flag indicating whether or not the section is ready for display.
 */
@property (nonatomic, readonly, getter=isReady) BOOL ready;

/*!
 @method     generateDivWithClass:id:content:
 @abstract   Convenience method for generating an HTML fragment consisting of a 
             div element with the given class and ID attributes, and the given 
             string content.
 @param      classValue
             The value for the div's class attribute.
 @param      idValue
             The value for the div's ID attribute.
 @param      contentValue
             The value for the div's body.
 @return     The new HTML fragment.
 */
+ (NSString *)generateDivWithClass:(NSString *)classValue id:(NSString *)idValue content:(NSString *)contentValue;

/*!
 @method     stringWithEscapedHTMLEntities:
 @abstract   Returns the string with escaped HTML entities.
 @param      string
             The HTML string to escape.
 @return     A copy of the string with appropriately-escaped HTML entities.
 */
+ (NSString *)stringWithEscapedHTMLEntities:(NSString *)string;

/*!
 @method     initWithTitle:identifier:
 @abstract   Creates and returns a new section initialized with the given title 
             and identifier.
 @param      title
             The title for the new section.
 @param      identifier
             The identifier for the new section.
 @return     The new section.
 */
- (id)initWithTitle:(NSString *)title identifier:(NSString *)identifier;

/*!
 @method     contentHTMLRepresentation
 @abstract   Returns an HTML fragment representing the section's contents.
 @return     An HTML fragment representing the section's contents.
 */
- (NSString *)contentHTMLRepresentation;

/*!
 @method     embeddedResources
 @abstract   An array of NSURL objects representing files written to a temporary
             directory for display in this section. These files will be deleted 
             when the section is deallocated.
 */
- (NSArray *)embeddedResources;

/*!
 @method     addEmbeddedResourceWithData:fileName:
 @abstract   Writes the provided data to file with the given path extension and 
             stores it as an embedded resource for this section.
 @param      resourceData
             The data to write to file.
 @param      pathExtension
             The path extension to use for the file, indicating the file type.
 @return     The URL to which the file was written.
 */
- (NSURL *)addEmbeddedResourceWithData:(NSData *)resourceData pathExtension:(NSString *)pathExtension;

/*!
 @method     renderWithSectionDelegate:
 @abstract   Begins the process of rendering the section's contents for display.
 @param      delegate
             The delegate object to notify when the section is ready for 
             display.
 */
- (void)renderWithSectionDelegate:(id<DetailsViewSectionDelegate>)delegate;

/*!
 @method     makeID:clickableToRange:
 @abstract   Registers the id to create a clickable range notification.
 @param      elementID
             The element to make clickable.
 @param      range
             The range of bytes in the original payload that the clickable 
             element represents.
 */
- (void)makeID:(NSString *)elementID clickableToRange:(NSRange)range;

/*!
 @method     elementIdentifier
 @abstract   Returns the HTML DOM element identifier for the div containing this 
             section.
 */
- (NSString *)elementIdentifier;

/*!
 @method     teardown
 @abstract   Prepares the section for being removed from view and deallocated.
 @discussion Subclasses that override this method should first invoke super's 
             implementation.
 */
- (void)teardown;

@end
