//
//  ProductPlanResistor.h
//  ATSMacApp
//
//  Created by Kip Nicol on 8/15/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <CaptureKit/ProductPlanType.h>

@interface ProductPlanResistor : ProductPlanType {
}

@property (nonatomic, readonly) ResistorType type;
@property (nonatomic, readonly) NSString *claimName;

+ (id)resistorWithResistorType:(ResistorType)type;
- (id)initWithResistorType:(ResistorType)type;

@end
