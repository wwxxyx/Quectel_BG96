//
//  L2CAPFrameEvent.h
//  ATSMacApp
//
//  Created by Edgard Lima on 08/13/18.
//  Copyright (c) 2018 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureKit.h>
#import "L2CAPFrame.h"

typedef NS_ENUM(NSInteger, L2CAPFrameSource) {
    /// Frame transmited by Apple Device (e.g. iPhone)
    L2CAPFrameSourceAppleDevice,
    /// Frame transmited by Accessory (e.g. CarPlay)
    L2CAPFrameSourceAccessory,
    /// Unknown source.
    L2CAPFrameSourceUnknown,
};

@interface L2CAPFrameEvent : CaptureEvent<ReportAnalyzerDisplayProtocol>

/** The L2CAPFrame object associated with the event. */
@property (nonatomic, strong) L2CAPFrame *l2capFrame;

/** Property that represents the source of the frame. */
@property (nonatomic, readonly, assign) L2CAPFrameSource source;

/** The index number to use in the frame description and represents the event count. */
@property (nonatomic) NSUInteger dataFrameIndex;

/** A string representation of the frame number. */
@property (nonatomic, readonly) NSString *frameNumberDescription;

/** A string representation of the Channel ID. */
@property (nonatomic, readonly) NSString *l2capChannelIdDescription;

/*!
 Returns object initialized to given parameters.
 @param timestamp The timestamp of the event.
 @param l2capFrame The associated L2CAPFrame object.
 @param source The source that transmit the event
 @return an instance of L2CAPFrameEvent object
 */
+ (instancetype)eventWithTimestamp:(UInt64)timestamp l2capFrame:(L2CAPFrame *)l2capFrame source:(L2CAPFrameSource)source;

/*!
 Initializes and returns object to given parameters.
 @param timestamp The timestamp of the event.
 @param l2capFrame The associated L2CAPFrame object.
 @param source The source that transmit the event
 @return an instance of L2CAPFrameEvent object
 */
- (instancetype)initWithTimestamp:(UInt64)timestamp l2capFrame:(L2CAPFrame *)l2capFrame source:(L2CAPFrameSource)source;

/*!
 Realizes the processing of the report analyzer entry.
 @param baseEntry an APReportAnalyzerEntry object that indicate its tree content
 */
- (void)reportAnalyzerEntryProcessing:(APReportAnalyzerEntry *)baseEntry;

@end
