//
//  TimestampTextField.h
//  ATSMacApp
//
//  Created by Wesley McCloy on 3/20/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface TimestampTextField : NSTextField

/**
 The backing timestamp (in ns) of the timestamp text field. Set this to update 
 the text field and read this to get the timestamp info.
 */
@property (nonatomic) uint64_t timestamp;


/**
 Method to reload the timestamp string when the timestamp units change
 */
- (void)timestampUnitsDidChange;

@end
