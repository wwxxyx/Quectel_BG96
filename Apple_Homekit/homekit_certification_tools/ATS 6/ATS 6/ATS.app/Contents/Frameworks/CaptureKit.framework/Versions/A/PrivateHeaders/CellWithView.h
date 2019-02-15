//
//  CellWithView.h
//  TestCustomCellzor
//
//  Created by Mark Hamlin on 7/14/11.
//  Copyright 2011 Apple. All rights reserved.
//

#import <AppKit/AppKit.h>

@interface CellWithView : NSActionCell {
    NSView *_view;
}
@property (nonatomic, retain) NSView *view;

@end
