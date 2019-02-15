//
//  CaptureConfigurationProtocolViewController.h
//  ATSMacApp
//
//  Created by Cody Brimhall on 7/23/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <CaptureKit/CaptureConfigurationAssistantViewController.h>

/*!
 Concrete subclass of the abstract class CaptureConfigurationAssistantViewController, with user interface elements for selecting the iAP protocol version to be used by the accessory in a capture.
 */
@interface CaptureConfigurationProtocolViewController : CaptureConfigurationAssistantViewController

/*!
 The matrix of radio buttons presenting the choice of protocol versions.
 */
@property (nonatomic, assign) IBOutlet NSMatrix *protocolMatrix;

/*!
 Check box indicating whether or not CarPlay is used
 */
@property (nonatomic, assign) IBOutlet NSButton *carPlayButton;

/*!
 Called by UI to refresh the data model's protocol mask to reflect current settings
 */
- (IBAction)updateProtocolMask:(id)sender;

@end
