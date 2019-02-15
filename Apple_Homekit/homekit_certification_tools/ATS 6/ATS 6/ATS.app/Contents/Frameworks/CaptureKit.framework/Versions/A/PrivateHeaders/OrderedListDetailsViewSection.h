//
//  OrderedListDetailsViewSection.h
//  ATSMacApp
//
//  Created by Kip Nicol on 4/1/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/DetailsViewSection.h>

@interface OrderedListDetailsViewSection : DetailsViewSection

/*!
 @property  listItems
 @abstract  Array of items to display in the ordered list.
 */
@property (nonatomic, readonly, copy) NSArray *listItems;

/*!
 @property  startingNumber
 @abstract  The number to start the list at. Defaults to 1.
 */
@property (nonatomic, assign) NSUInteger startingNumber;

/*!
 @method    initWithListItems:title:identifier:
 @abstract  Inits and returns a OrderedListDetailsViewSection with the given parameters.
 @param     listItems
            Array of items to display in the ordered list.
 @param     title
            The title of the section.
 @param     identifier
            The identifier for the section.
 */
- (instancetype)initWithListItems:(NSArray *)listItems title:(NSString *)title identifier:(NSString *)identifier;

@end
