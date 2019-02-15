//
//  iAP2LinkPacketsTrafficViewController.h
//  ATSMacApp
//
//  Created by Douglas Frari on 02/07/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureKit.h>
#import <CaptureKit/ReportAnalyzerBridgeTrafficViewController.h>


@interface iAP2LinkPacketsTrafficViewController : ReportAnalyzerBridgeTrafficViewController

/**
Initializes with a particular transport name, used for the title
@param transportName the transport name for the title
 @return instancetype of iAP2LinkPacketsTrafficViewController
*/
+ (instancetype)initWithTransportName:(NSString *)transportName;


@end
