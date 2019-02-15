//
//  SummaryCaptureTransport.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 4/30/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureTransport.h>

@class MeasurementsViewController;
@class SummaryGroupTrafficViewController;

@interface SummaryCaptureTransport : CaptureTransport

@property (nonatomic, readonly, retain) MeasurementsViewController *measurementsTrafficViewController;

/*!
 @property  iAPGroupTrafficViewController
 @abstract  iAP1 group traffic view controller
 */
@property (nonatomic, readonly, retain) SummaryGroupTrafficViewController *iAPGroupTrafficViewController;

/*!
 @property  iAP2GroupTrafficViewController
 @abstract  iAP2 group traffic view controller
 */
@property (nonatomic, readonly, retain) SummaryGroupTrafficViewController *iAP2GroupTrafficViewController;

/*!
 @property  hardwareGroupTrafficViewController
 @abstract  Hardware event group traffic view controller
 */
@property (nonatomic, readonly, retain) SummaryGroupTrafficViewController *hardwareGroupTrafficViewController;

@end
