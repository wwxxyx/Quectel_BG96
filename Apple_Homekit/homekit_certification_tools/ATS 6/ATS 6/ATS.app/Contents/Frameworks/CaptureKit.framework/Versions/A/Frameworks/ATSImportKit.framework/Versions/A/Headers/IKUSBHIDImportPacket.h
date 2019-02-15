//
//  IKUSBHIDImportPacket.h
//  ATSImportKit
//
//  Created by Mark Hamlin on 5/8/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import "IKImportPacket.h"

@interface IKUSBHIDImportPacket : IKImportPacket

/*!
 @property  hidReportData
 @abstract  The data for the HID report
 */
@property (nonatomic, retain) NSData *hidReportData;

@end
