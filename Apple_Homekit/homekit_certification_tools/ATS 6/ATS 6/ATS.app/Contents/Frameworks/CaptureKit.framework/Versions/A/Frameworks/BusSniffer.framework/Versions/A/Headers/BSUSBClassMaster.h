//
//  BSUSBClassMaster.h
//  BusSniffer
//
//  Created by Joe Basu on 11/6/09.
//  Copyright © 2009 Apple Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>


@interface BSUSBClassMaster : NSObject {

}

+ (NSString *)classNameForClassCode:(UInt8)classCode;

@end
