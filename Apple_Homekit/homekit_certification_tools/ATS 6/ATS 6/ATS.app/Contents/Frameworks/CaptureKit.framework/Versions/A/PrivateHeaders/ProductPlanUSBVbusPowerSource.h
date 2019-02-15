//
//  ProductPlanUSBVbusPowerSource.h
//  ATSMacApp
//
//  Created by Kip Nicol on 8/11/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/ProductPlanType.h>

@interface ProductPlanUSBVbusPowerSource : ProductPlanType {
}

@property (nonatomic, readonly) USBVbusPowerSourceType type;
@property (nonatomic, readonly) NSString *claimName;

+ (id)usbVbusPowerSourceWithUSBVbusPowerSourceType:(USBVbusPowerSourceType)type;
- (id)initWithUSBVbusPowerSourceType:(USBVbusPowerSourceType)type;

@end
