//
//  OptionsSearchField.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 8/9/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface OptionsSearchField : NSSearchField

/*!
 @property  optionsViewController
 @abstract  View controller to go into the popover
 */
@property (nonatomic, assign) IBOutlet NSViewController *optionsViewController;

@end
