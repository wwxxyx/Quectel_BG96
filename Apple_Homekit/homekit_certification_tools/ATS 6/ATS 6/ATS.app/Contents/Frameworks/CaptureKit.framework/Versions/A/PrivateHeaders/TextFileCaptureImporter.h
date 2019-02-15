//
//  TextFileCaptureImporter.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/10/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSCaptureImporter.h>

#import <BusSniffer/BusSniffer.h>
#import <CaptureKit/CaptureConfiguration.h>

@interface TextFileCaptureImporter : ATSCaptureImporter

/*!
 @property  transport
 @abstract  Target transport for import
 */
@property (nonatomic, assign) BSiAPTransport transport;

/*!
 @property  isiAP2
 @abstract  Whether or not the trace uses iAP2
 */
@property (nonatomic, assign) BOOL isiAP2;

/*!
 @method    initWithURL:
 @abstract  Initialize with the text file at the passed URL
 */
- (id)initWithURL:(NSURL *)url;

@end
