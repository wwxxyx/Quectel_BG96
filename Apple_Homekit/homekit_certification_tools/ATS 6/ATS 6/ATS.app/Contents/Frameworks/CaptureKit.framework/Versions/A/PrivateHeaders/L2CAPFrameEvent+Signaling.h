//
//  L2CAPFrameEvent+Signaling.h
//  ATSMacApp
//
//  Created by Douglas Frari on 27/08/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureKit.h>
#import <ComprobeSupport/ComprobeSupport.h>
#import "L2CAPFrame.h"
#import "L2CAPFrameEvent.h"

@interface L2CAPFrameEvent (Signaling)

/*!
 Verifies the Signaling description sceneries and prepares the result customization entries for detail screen.
 @param channelID The channel ID code.
 @param entry a APReportAnalyzerEntry object. This is optional and indicates that the detail screen has customize visualization.  The parameter entry can be nil. 
 @return A string that is showed on TrafficKit view detail tree.
 */
- (void)signalingDetailDescriptionWithChannelId:(L2CAPChannelId )channelID andReportAnalyserEntry:(APReportAnalyzerEntry *)entry;

/*!
 Verifies the Signaling description sceneries and prepares the result column value.
 @param channelID The channel ID code.
 @return A string that is showed on TrafficKit view column.
 */
- (NSString *)signalingColumnDescriptionWithChannelId:(L2CAPChannelId )channelID;

@end
