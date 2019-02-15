//
//  ATSHciContainerFileImporterWindowController.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 1/28/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class ATSHciFileImporter;

/** ATSHciContainerFileImporterWindowController manages the window with configuration options for importing iAP data from a PacketLogger trace. */
@interface ATSHciContainerFileImporterWindowController : NSWindowController <NSWindowDelegate, NSTableViewDataSource, NSTableViewDelegate>

/** A flag indicating whether or not to treat the accessory as the ACL sender. */
@property BOOL accessoryIsSender;

/** A flag indicating whether or not the source file contains iAP1 data. */
@property BOOL containsiAP1Data;

/**
 Initializes and returns a new window controller configured to use the given importer for processing source data.
 
 @param importer The PacketLogger importer to use for processing source data.
 @return The new window controller.
 */
- (instancetype)initWithImporter:(ATSHciFileImporter *)importer;

/** Presents the import configuration sheet. */
- (void)show;

@end
