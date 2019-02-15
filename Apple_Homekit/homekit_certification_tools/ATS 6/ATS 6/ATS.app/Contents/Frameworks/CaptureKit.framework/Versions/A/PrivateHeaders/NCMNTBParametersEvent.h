//
//  NCMNTBParametersEvent.h
//  ATSMacApp
//
//  Created by Mark Hamlin on 5/20/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <CaptureKit/NCMEvent.h>
#import <NCMKit/NCMKit.h>

@interface NCMNTBParametersEvent : NCMEvent

/*!
 @property  parameters
 @abstract  The parameters for NCM
 */
@property (nonatomic, assign) NCMGetNTBParametersStructure parameters;

@end
