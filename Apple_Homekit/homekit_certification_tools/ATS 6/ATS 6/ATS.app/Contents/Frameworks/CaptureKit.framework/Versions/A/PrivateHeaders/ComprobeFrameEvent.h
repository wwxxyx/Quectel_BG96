//
//  ComprobeFrameEvent.h
//  ATSMacApp
//
//  Created by Kip Nicol on 1/9/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent.h>
#import <ComprobeSupport/ComprobeSupport.h>
#import <CaptureKit/CaptureKit.h>

@interface ComprobeFrameEvent : CaptureEvent<ReportAnalyzerDisplayProtocol>

/*!
 @property  comprobeFrame
 @abstract  The ComprobeFrame object associated with the event.
 */
@property (nonatomic, strong) ComprobeFrame *comprobeFrame;

/** Errors if any */
@property (nonatomic, strong) NSArray<NSError *> *errors;

/*!
 The timestamp of the event.
 @param timestamp The timestamp of the event.
 @param comprobeFrame The associated ComprobeFrame object.
 @return an instance of ComprobeFrameEvent object
 */
+ (instancetype)eventWithTimestamp:(UInt64)timestamp
                     comprobeFrame:(ComprobeFrame *)comprobeFrame;

/*!
 The timestamp of the event.
 @param timestamp The timestamp of the event.
 @param comprobeFrame The associated ComprobeFrame object.
 @return an instance of ComprobeFrameEvent object
 */
- (instancetype)initWithTimestamp:(UInt64)timestamp
                    comprobeFrame:(ComprobeFrame *)comprobeFrame;

/** A string representation of the source of the packet. */
@property (nonatomic, readonly) NSString *sourceDescription;

/** A string representation of the packet status. */
@property (nonatomic, readonly) NSString *packetStatusDescription;

/** A string representation of the frame number. */
@property (nonatomic, readonly) NSString *frameNumber;

/** A string representation of the Bluetooth clock. */
@property (nonatomic, readonly) NSString *clockDescription;

/** A string representation of the link flow. */
@property (nonatomic, readonly) NSString *linkFlowDescription;

/** A string representation of the packet type */
@property (nonatomic, readonly) NSString *packetTypeDescription;

/** A string representation of the active member address. */
@property (nonatomic, readonly) NSString *activeMemberAddressDescription;

/** A string representation of the SEQN. */
@property (nonatomic, readonly) NSString *seqnDescription;

/** A string representation of the ARQN. */
@property (nonatomic, readonly) NSString *arqnDescription;

/** A string representation of the L2CAP flow. */
@property (nonatomic, readonly) NSString *l2capFlowDescription;

/** A string representation of the link. */
@property (nonatomic, readonly) NSString *logicalLinkDescription;

/** A string representation of the frame length. */
@property (nonatomic, readonly) NSString *frameLengthDescription;

/*!
 Realizes the processing of the report analyzer entry. This method can be used by other
 layers to display its contents.
 @param baseEntry an APReportAnalyzerEntry object that indicate its tree content
 */
- (void)reportAnalyzerEntryProcessing:(APReportAnalyzerEntry *)baseEntry;

@end
