//
//  LKTInstance.h
//  LingoKit
//
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>


@interface LKTInstance : NSObject <NSCopying, NSCoding> {
    NSString *_desc;
}

@property (nonatomic, copy) NSString *desc;
@property (nonatomic, readonly) NSString *formattedDescription;
@property (nonatomic, readonly) NSString *templateDescription;

@end
