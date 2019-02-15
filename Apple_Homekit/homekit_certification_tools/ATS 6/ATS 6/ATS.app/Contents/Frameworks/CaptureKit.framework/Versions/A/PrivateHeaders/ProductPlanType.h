//
//  ProductPlanType.h
//  ATSMacApp
//
//  Created by Kip Nicol on 8/11/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CaptureKit/ProductPlanTypes.h>

@interface ProductPlanType : NSObject <NSCoding> {
    NSUInteger _type;
}

@property (nonatomic, readonly) NSString *name;

/*!
 @property  isDeprecated
 @abstract  Whether the Product Plan object is deemed "deprecated"
 @discussion An example for this case is when an Apple device that used to be supported
             in the CA is no longer supported.
 */
@property (nonatomic, readonly) BOOL isDeprecated;

- (id)initWithType:(NSUInteger)type;

@end
