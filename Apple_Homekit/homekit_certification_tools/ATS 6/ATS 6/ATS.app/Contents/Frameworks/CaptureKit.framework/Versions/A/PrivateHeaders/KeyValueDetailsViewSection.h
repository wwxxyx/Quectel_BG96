//
//  KeyValueDetailsViewSection.h
//  ATS
//
//  Created by Joe Basu on 6/1/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/DetailsViewSection.h>

@interface KeyValueDetailsViewSection : DetailsViewSection {
    NSArray *_fields;
}

@property (nonatomic, copy) NSArray *fields;

@end
