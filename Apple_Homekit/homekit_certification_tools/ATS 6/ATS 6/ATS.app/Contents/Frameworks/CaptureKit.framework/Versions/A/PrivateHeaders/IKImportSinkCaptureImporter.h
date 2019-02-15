//
//  IKImportSinkCaptureImporter.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/13/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/ATSCaptureImporter.h>
#import <ATSImportKit/ATSImportKit.h>

@interface IKImportSinkCaptureImporter : ATSCaptureImporter <IKSourcePacketizerDelegate>

/*!
 @property  transportMask
 @abstract  The mask for the transports the importer should handle
 */
@property (nonatomic, assign) IKImportTransport transportMask;

/*!
 @property  protocolMask
 @abstract  The mask for the protocols the importer should handle
 */
@property (nonatomic, assign) IKImportProtocol protocolMask;

@end
