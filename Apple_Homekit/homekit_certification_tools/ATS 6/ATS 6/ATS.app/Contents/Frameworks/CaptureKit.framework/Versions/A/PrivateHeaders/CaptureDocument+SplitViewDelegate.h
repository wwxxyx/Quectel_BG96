//
//  CaptureDocument+SplitViewDelegate.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/7/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureDocument.h>

@interface CaptureDocument (SplitViewDelegate)

/*!
 @method    toggleCategoriesShowing
 @abstract  Toggle the transport and traffic category on the left of the window
 */
- (void)toggleCategoriesShowing;

- (BOOL)isCategoriesViewShowing;
@end
