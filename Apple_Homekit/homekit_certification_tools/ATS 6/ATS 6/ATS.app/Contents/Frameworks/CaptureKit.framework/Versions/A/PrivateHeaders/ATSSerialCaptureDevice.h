//
//  ATSSerialCaptureDevice.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 10/16/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureDevice+Protected.h>

@class ATSSerialCaptureDevice;

@protocol ATSSerialCaptureDeviceDelegate <NSObject>
@required
/**
 This delegate gets called when there is some serial data seen on the ATS Serial Capture device. The capture device should be connected and instructed to start sniffing before expecting this delegate to be called.
 
 @param device ATS serial capture device instance.
 @param atsSerialData Serial data sniffed from the capture device.
 
 @return Return an array of Capture events created after reading the serial data.
 */
- (NSArray *)atsSerialCaptureDevice:(ATSSerialCaptureDevice *)device sawSerialData:(NSData *)atsSerialData;

/**
 This delegate gets called when ATS Serial Capture device becomes invalid. i.e, the box was physically disconnected or if there has been some issue with the connection.

 @param device ATS serial capture device instance.
 */
- (void)atsSerialCaptureDeviceDidBecomeInvalid:(ATSSerialCaptureDevice *)device;

@end


@interface ATSSerialCaptureDevice : CaptureDevice

/**
 Serial capture device path found by IO notifier. Eg: /dev/cu.usb-1234ABCD
 */
@property (nonatomic, copy) NSString *devicePath;

/**
 Location ID of the serial capture device found by IO notifier.
 */
@property (nonatomic, copy) NSNumber *locationId;

/**
 Product ID the ATF R3 compliant box. Key [idProduct] in the device tree
 */
@property (nonatomic) NSUInteger productID;

/**
 Name of the ATS product compliant with ATF R3.
 */
@property (nonatomic, readonly) NSString *productName;

/**
 Implement the delegate method to be able to receive sniffed data. In ATS, capture transports are the delegates that receive the data and create capture events.
 */
@property (nonatomic, assign) id<ATSSerialCaptureDeviceDelegate> delegate;

/**
 Serial queue to be able to synchorize the sniffed bytes.
 */
@property (nonatomic, strong) dispatch_queue_t serialSyncQueue;

/**
 Instantiate a serial capture device with input serial number and device path.
 
 @param serialNumber Serial number of the serial capture device.
 @param devicePath Device path of the serial capture device.
 */
+ (instancetype)deviceWithSerialNumber:(NSString *)serialNumber devicePath:(NSString *)devicePath;

/**
 Instantiate a serial capture device with input serial number and device path.
 
 @param serialNumber Serial number of the serial capture device.
 @param devicePath Device path of the serial capture device.
 */
- (instancetype)initWithSerialNumber:(NSString *)serialNumber devicePath:(NSString *)devicePath;

@end
