//
//  ATSHciFileImporter+Protected.h
//  CaptureKit
//
//  Created by Vinod Madigeri on 11/26/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSHciFileImporter.h>

@interface ATSHciFileImporter ()
@property (readwrite) ATSHciImporter *hciImporter;
@property (readwrite) unsigned long long fileSize;
@property (nonatomic, readwrite) unsigned long long fileLoadOffset;
@property (readwrite) NSURL *url;
@property (readwrite) double elapsedReadTime;
@property (readwrite) NSArray *rfcommStreams;
@end
