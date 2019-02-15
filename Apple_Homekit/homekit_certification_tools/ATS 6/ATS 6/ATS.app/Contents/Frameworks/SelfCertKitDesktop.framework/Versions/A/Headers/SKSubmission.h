//
//  SKSubmission.h
//  
//
//  Created by Zachary Church on 9/21/15.
//  Copyright © 2015 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SKModel.h"

@interface SKSubmission : SKModel

+ (instancetype)submissionWithDictionary:(NSDictionary *)dictionary;

@property (strong, nonatomic) NSString *state;
@property (strong, nonatomic) NSString *uuid;
@property (strong, nonatomic) NSArray *attachments;

@end
