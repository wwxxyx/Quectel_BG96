//
//  IDBusPacketEvent.h
//  ATSMacApp
//
//  Created by Kip Nicol on 10/18/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <CaptureKit/CaptureEvent+Protected.h>
#import <IDBusKit/IDBusKit.h>

@interface IDBusPacketEvent : CaptureEvent <IDBusDisplay>

@property (nonatomic, retain) IDBPacket *idBusPacket;
@property (nonatomic, readonly) IDBCommand command;
@property (nonatomic, readonly) IDBPacketError errorCode;
@property (nonatomic, readonly) NSNumber *numberValue;

/*!
 @property  stringValue
 @abstract  Returns the stringValue from the @c idBusPacket object.
 */
@property (nonatomic, readonly) NSString *stringValue;

/*!
 @method    errorCodeString
 @abstract  String representation of the error code
 */
- (NSString *)errorCodeString;

@end
