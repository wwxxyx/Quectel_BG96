//
//  EventChanneling.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 5/14/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol EventChanneling <NSObject, NSCopying>

/**
 Identifier for this channel. Each subscriber may care of specific event channel for specific events.
 */
@property (nonatomic, readonly) NSUInteger identifier;

/**
 Channel name.
 */
@property (nonatomic, strong, readonly) NSString *name;

@end
