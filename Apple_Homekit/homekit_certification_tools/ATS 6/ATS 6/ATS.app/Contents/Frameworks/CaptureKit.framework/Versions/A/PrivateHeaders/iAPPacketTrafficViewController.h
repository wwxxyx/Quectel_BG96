//
//  iAPPacketTrafficViewController.h
//  ATSMacApp
//
//  Created by Bob Burrough on 12/12/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/iAPTrafficViewController.h>

@interface iAPPacketTrafficViewController : iAPTrafficViewController

/*!
 @method    initWithTransportName:
 @abstract  Initializes with a particular transport name, used for the title
 */
- (id)initWithTransportName:(NSString *)transportName;

@end
