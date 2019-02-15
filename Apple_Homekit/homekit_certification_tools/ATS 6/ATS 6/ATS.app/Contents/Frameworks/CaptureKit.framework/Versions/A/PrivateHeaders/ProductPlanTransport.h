//
//  ProductPlanTransport.h
//  ATSMacApp
//
//  Created by Kip Nicol on 8/11/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/ProductPlanType.h>

@interface ProductPlanTransport : ProductPlanType {
}

@property (nonatomic, readonly) TransportType type;
@property (nonatomic, readonly) BOOL isUSBTransport;

+ (id)transportWithTransportType:(TransportType)type;
- (id)initWithTransportType:(TransportType)type;

@end
