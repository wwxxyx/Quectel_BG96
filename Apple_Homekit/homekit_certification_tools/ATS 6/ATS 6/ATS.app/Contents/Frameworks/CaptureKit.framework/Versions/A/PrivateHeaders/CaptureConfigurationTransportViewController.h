//
//  CaptureConfigurationTransportViewController.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 8/3/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

/*!
 @header     CaptureConfigurationTransportDevicesViewController
 @abstract   This header file describes the interface for the view controller
             responsible for configuring a capture configuration data model's
             transport layer properties.
 */

#import <CaptureKit/CaptureConfigurationAssistantViewController.h>

/*!
 @class      CaptureConfigurationTransportViewController
 @abstract   Concrete subclass of the abstract class
             CaptureConfigurationAssistantViewController, with user interface
             elements for selecting the transport layer.
 */
@interface CaptureConfigurationTransportViewController : CaptureConfigurationAssistantViewController

/*!
 @property   availableTransports
 @abstract   The list of transports applicable to the connector and iAP version 
             the data model is currently configured for.
 */
@property (nonatomic, copy, readonly) NSArray *availableTransports;

@end
