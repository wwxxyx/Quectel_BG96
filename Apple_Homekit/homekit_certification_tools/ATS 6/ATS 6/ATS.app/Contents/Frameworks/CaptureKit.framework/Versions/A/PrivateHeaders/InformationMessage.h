//
//  InformationMessage.h
//  ATSMacApp
//
//  Created by Douglas Frari on 26/04/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface InformationMessage : NSObject

/*!
 The timestamp when the Event sentence was received
 */
@property (nonatomic) UInt64 timestamp;


/*!
 Initializes a new InformationMessage with the specified timestamp
 @param timestamp  timestamp when the LocationInformation message was received
 @return a new InformationMessage
 */
- (instancetype)initWithTimestamp:(UInt64)timestamp;

@end
