//
//  CaptureDocument+Annotations.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/7/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureDocument.h>

@interface CaptureDocument (Annotations)

- (IBAction)menuInsertSeparatorAbove:(id)sender;
- (IBAction)menuInsertSeparatorBelow:(id)sender;
- (IBAction)menuInsertCommentAbove:(id)sender;
- (IBAction)menuInsertCommentBelow:(id)sender;

@end
