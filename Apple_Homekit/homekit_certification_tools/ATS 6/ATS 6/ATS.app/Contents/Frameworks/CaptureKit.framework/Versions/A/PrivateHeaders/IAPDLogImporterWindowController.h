//
//  IAPDLogImporterWindowController.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 3/27/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface IAPDLogImporterWindowController : NSWindowController <NSTableViewDataSource, NSTableViewDelegate>

/*!
 @method    initWithLog:
 @abstract  Initialize with the URL of a log
 @return    self on success, nil if log could not be opened
 */
- (id)initWithLog:(NSURL *)url;

/*!
 @method    show
 @abstract  Presents import sheet
 */
- (void)show;

@end
