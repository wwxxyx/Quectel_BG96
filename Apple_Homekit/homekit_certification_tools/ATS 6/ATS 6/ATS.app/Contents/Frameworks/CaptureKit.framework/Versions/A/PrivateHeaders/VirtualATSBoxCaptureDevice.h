//
//  VirtualATSBoxCaptureDevice.h
//  ATS
//
//  Created by Joe Basu on 5/7/10.
//  Copyright © 2010 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/ATSBoxCaptureDevice.h>

@interface VirtualATSBoxCaptureDevice : ATSBoxCaptureDevice {
    NSString *_identifier;
    NSConnection *_connection;
}

@property (nonatomic, copy, readonly) NSString *identifier;
@property (nonatomic, copy, readonly) NSString *portName;
@property (nonatomic, retain, readonly) NSConnection *connection;

+ (id)deviceWithIdentifier:(NSString *)identifier portName:(NSString *)portName;
- (id)initWithIdentifier:(NSString *)identifier portName:(NSString *)portName;

@end
