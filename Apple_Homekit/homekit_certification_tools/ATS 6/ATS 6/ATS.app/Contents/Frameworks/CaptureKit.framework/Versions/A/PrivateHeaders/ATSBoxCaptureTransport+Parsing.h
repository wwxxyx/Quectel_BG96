//
//  ATSBoxCaptureTransport+Parsing.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/1/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSBoxCaptureTransport.h>

@interface ATSBoxCaptureTransport (Parsing)

- (void)processATF:(NSData *)atfData;

@end
