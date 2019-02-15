//
//  USBCSVFileImporterWindowController.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/7/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface USBCSVFileImporterWindowController : NSWindowController

/*!
 @method    initWithCSV:
 @abstract  Initialize with the URL of a csv trace
 */
- (id)initWithCSV:(NSURL *)url;

/*!
 @method    show
 @abstract  Presents import sheet
 */
- (void)show;

@end
