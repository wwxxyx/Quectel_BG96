//
//  SKProductPlan.h
//  
//
//  Created by Zachary Church on 9/18/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SKModel.h"

@interface SKProductPlan : SKModel

@property (strong, nonatomic) NSString *ppid;
@property (strong, nonatomic) NSString *sku;
@property (strong, nonatomic) NSArray *transports;
@property (strong, nonatomic) NSArray *technologies;
@property (strong, nonatomic) NSArray *protocols;
@property (strong, nonatomic) NSString *name;
@property (strong, nonatomic) NSString *state;
@property (strong, nonatomic) NSArray *supported_devices;
@property (strong, nonatomic) NSArray *features;
@property (strong, nonatomic) NSArray *device_types;
@property (strong, nonatomic) NSString *account_specialist;
@property (strong, nonatomic) NSArray *connectors;
@property (strong, nonatomic) NSString *ios_apps;

+ (instancetype)productPlanWithDictionary:(NSDictionary *)dictionary;

@end
