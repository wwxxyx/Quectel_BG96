//
//  BeagleUSBCaptureDevice+Protected.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 3/7/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/BeagleUSBCaptureDevice.h>

@class BeagleUSBCaptureDevice;

@protocol BeagleUSBCaptureDeviceDelegate <NSObject>

@required

- (NSArray *)beagleUSBCaptureDevice:(BeagleUSBCaptureDevice *)device
                          sawStatus:(UInt32)status
                             events:(UInt32)events
                            timeSOP:(UInt64)timeSOP
                       timeDuration:(UInt64)timeDuration
                     timeDataOffset:(UInt32)timeDataOffset
                        packetBytes:(const void *)packetBytes
                       packetLength:(UInt16)packetLength;

- (void)beagleUSBCaptureDeviceDidBecomeInvalid:(BeagleUSBCaptureDevice *)device;

@end

@interface BeagleUSBCaptureDevice ()

@property (nonatomic, weak) id<BeagleUSBCaptureDeviceDelegate> delegate;

@property (nonatomic) UInt32 sampleRate;

@end
