//
//  CaptureItemsDetailsViewSection.h
//  ATS
//
//  Created by Marin Balde on 7/26/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <CaptureKit/DetailsViewSection.h>

@interface CaptureItemsDetailsViewSection : DetailsViewSection {
    NSArray *_captureItems;
}

@property (nonatomic, readwrite, retain) NSArray *captureItems;

@end
