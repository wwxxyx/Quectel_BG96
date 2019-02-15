//
//  PerformanceEventConfig.h
//  ATSMacApp
//
//  Created by Wesley McCloy on 7/27/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface PerformanceEventConfig : NSObject

@property NSString *title;
@property NSString *childTitle;
@property BOOL coloredDetails;
@property BOOL coloredRows;
@property NSNumber *numberOfChildren;

@end
