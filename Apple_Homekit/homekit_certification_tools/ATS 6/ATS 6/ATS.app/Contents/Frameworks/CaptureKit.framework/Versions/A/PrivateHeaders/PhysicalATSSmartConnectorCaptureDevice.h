//
//  PhysicalATSSmartConnectorCaptureDevice.h
//  ATSMacApp
//
//  Created by Zachary Church on 7/23/15.
//  Copyright (c) 2015 Apple Inc. All rights reserved.
//

#import <CaptureKit/PhysicalATFR3CaptureDevice.h>

/**
 Product ID of the Smart Connector Box when it's in serial mode
 */
extern const NSUInteger ATSSmartConnectorBoxProductId;

/**
 Vendor ID of the Smart Connector Box when it's in serial mode. (Apple Inc.)
 */
extern const NSUInteger ATSSmartConnectorBoxVendorId;

/**
 Product ID of the Smart Connector Box when it's in DFU mode
 */
extern const NSUInteger ATSSmartConnectorBoxDFUModeProductId;


@interface PhysicalATSSmartConnectorCaptureDevice : PhysicalATFR3CaptureDevice

@end
