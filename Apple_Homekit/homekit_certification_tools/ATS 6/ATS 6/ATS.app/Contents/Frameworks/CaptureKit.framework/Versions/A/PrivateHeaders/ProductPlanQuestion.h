//
//  ProductPlanQuestion.h
//  ATSMacApp
//
//  Created by Kip Nicol on 8/11/11.
//  Copyright (c) 2011 Apple Inc. All rights reserved.
//

#import <CaptureKit/ProductPlanType.h>

@interface ProductPlanQuestion : ProductPlanType {
}

+ (id)questionWithQuestionType:(QuestionType)type;
- (id)initWithQuestionType:(QuestionType)type;

@end
