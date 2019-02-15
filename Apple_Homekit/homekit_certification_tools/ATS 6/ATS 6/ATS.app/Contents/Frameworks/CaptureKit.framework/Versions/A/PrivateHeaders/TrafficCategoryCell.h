//
//  TrafficCategoryCell.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 3/22/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface TrafficCategoryCell : NSTextFieldCell

@property (nonatomic, assign) NSUInteger numWarnings;
@property (nonatomic, assign) NSUInteger numErrors;

@end
