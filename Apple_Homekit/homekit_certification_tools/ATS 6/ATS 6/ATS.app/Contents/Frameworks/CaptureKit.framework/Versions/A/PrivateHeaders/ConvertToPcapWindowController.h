//
//  ConvertToPcapWindowController.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 6/3/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ConvertToPcapWindowController : NSWindowController

/*!
 @method    convertFrom:to:
 @abstract  Converts the trace at the first specified URL to a new pcap trace at the second specified URL
 */
+ (ConvertToPcapWindowController *)convertFrom:(NSURL *)atsURL to:(NSURL *)pcapURL;

@end
