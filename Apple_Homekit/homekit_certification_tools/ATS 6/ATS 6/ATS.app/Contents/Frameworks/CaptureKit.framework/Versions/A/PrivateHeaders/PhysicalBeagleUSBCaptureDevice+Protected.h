//
//  PhysicalBeagleUSBCaptureDevice+Protected.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 3/7/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/PhysicalBeagleUSBCaptureDevice.h>

@interface PhysicalBeagleUSBCaptureDevice (Protected)

+ (instancetype)deviceWithSerialNumber:(NSString *)serialNumber usbProductName:(NSString *)productName beagleType:(ATSUSBBeagleType)beagleType poshBeagle:(BOOL)poshBeagle;

/*!
 Retrieves the current firmware version from the Beagle.
 @param     majorVersion
 On return contains the major firmware version.
 @param     minorVersion
 On return contains the minor firmware version.
 @param     error
 On return contains the error if the return value was NO;
 @return    YES when firmware version was retrieved successfully, NO otherwise.
 */
- (BOOL)getFirmwareVersionMajor:(UInt8 *)majorVersion minor:(UInt8 *)minorVersion error:(NSError **)error;

@end
