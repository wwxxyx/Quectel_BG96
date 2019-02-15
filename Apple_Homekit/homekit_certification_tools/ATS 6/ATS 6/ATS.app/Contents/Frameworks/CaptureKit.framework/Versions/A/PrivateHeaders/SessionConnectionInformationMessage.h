//
//  SessionConnectionInformationMessage.h
//  ATSMacApp
//
//  Created by Douglas Frari on 26/04/18.
//  Copyright © 2018 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "InformationMessage.h"

@interface SessionConnectionInformationMessage : InformationMessage


/*!
 @property  connectionStatusMessage
 @abstract  A Connection-event message indicating when a connection might have occured for the event
 */
@property (nonatomic, readonly) NSString *connectionStatusMessage;


/*!
 Instantiates an event to describe a CarPlay Connection-Event status message.
 
 @param timestamp Timestamp of when the connection started or finished.
 @param connectionStatusMessage The Connection-event status message of when the CarPlay accessory connected or disconnected.
 
 @return An instance of CarPlaySessionConnectionEvent.
 */
- (instancetype)initWithTimestamp:(UInt64)timestamp connectionStatusMessageEvent:(NSString *)connectionStatusMessage;

@end
