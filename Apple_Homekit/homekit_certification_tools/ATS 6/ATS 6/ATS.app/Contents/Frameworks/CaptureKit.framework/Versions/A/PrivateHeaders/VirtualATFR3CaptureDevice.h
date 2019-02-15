//
//  VirtualATFR3CaptureDevice.h
//  ATSR3MacApp
//
//  Created by Marin Balde on 4/11/12.
//  Copyright © 2012 Apple Inc. All Rights Reserved.
//

#import <CaptureKit/ATSSerialCaptureDevice.h>

@interface VirtualATFR3CaptureDevice : ATSSerialCaptureDevice {
    NSString *_identifier;
    NSConnection *_connection;
}

@property (nonatomic, copy, readonly) NSString *identifier;
@property (nonatomic, copy, readonly) NSString *portName;
@property (nonatomic, retain, readonly) NSConnection *connection;

+ (instancetype)deviceWithIdentifier:(NSString *)identifier withName:(NSString *)deviceName andPortName:(NSString *)portName;

- (instancetype)initWithIdentifier:(NSString *)identifier withName:(NSString *)deviceName andPortName:(NSString *)portName;

@end
