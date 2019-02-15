//
//  GroupDetailsViewSection.h
//  ATSMacApp
//
//  Created by Joe Basu on 7/3/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

/*!
 @header     DetailsViewSection
 @discussion This header file describes the interface for the class that 
             represents a group of sections within the details sidebar in a 
             trace window.
 */

#import <CaptureKit/DetailsViewSection.h>

/*!
 @class      GroupDetailsViewSection
 @abstract   GroupDetailsViewSection manages a collection of subsections within 
             a details view tree.
 */
@interface GroupDetailsViewSection : DetailsViewSection <DetailsViewSectionDelegate>

/*!
 @method     addSubsection:
 @abstract   Adds a subsection to the group.
 @param      subsection
             The section to add as a subsection of the group.
 */
- (void)addSubsection:(DetailsViewSection *)subsection;

/*!
 @method     subsectionAtIndex:
 @abstract   Returns the subsection at the given index.
 @param      subsectionIndex
             The index of the subsection to return.
 @return     The subsection at the given index.
 */
- (DetailsViewSection *)subsectionAtIndex:(NSUInteger)subsectionIndex;

/*!
 @method     subsectionCount
 @abstract   Returns the number of subsections contained in the group.
 @return     The number of subsections within the group.
 */
- (NSUInteger)subsectionCount;

@end
