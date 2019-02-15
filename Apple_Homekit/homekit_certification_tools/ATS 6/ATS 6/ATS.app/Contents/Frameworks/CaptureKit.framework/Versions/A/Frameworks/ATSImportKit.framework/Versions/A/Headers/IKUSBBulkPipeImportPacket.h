//
//  IKUSBBulkPipeImportPacket.h
//  ATSImportKit
//
//  Created by Mark Hamlin on 5/8/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import "IKImportPacket.h"

@interface IKUSBBulkPipeImportPacket : IKImportPacket

/*!
 @property  transferData
 @abstract  The data for the USB transfer
 */
@property (nonatomic, retain) NSData *transferData;

@end
