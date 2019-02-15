//
//  CaptureDocument+TimingInspector.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/7/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureDocument.h>

@interface CaptureDocument (TimingInspector)

- (void)setStartItemFromSelection:(id)sender;
- (void)clearStartItem:(id)sender;
- (void)jumpToStartItem:(id)sender;

- (void)setEndItemFromSelection:(id)sender;
- (void)clearEndItem:(id)sender;
- (void)jumpToEndItem:(id)sender;

- (void)updateTimingInspectorStartItem;
- (void)updateTimingInspectorEndItem;

@end
