//
//  I2MMessageDefinition.h
//  iAP2MessageKit
//
//  Created by Joe Basu on 4/26/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <iAP2MessageKit/I2MDefinition.h>
#import <iAP2MessageKit/I2MTypes.h>

/*!
 @class     I2MMessageDefinition
 @abstract  Class for definitions of iAP2 messages
 */

@interface I2MMessageDefinition : I2MDefinition <NSCoding, NSCopying>

/*!
 @property identifier
 @abstract The message ID
 */
@property (nonatomic, readonly) UInt16 identifier;

/*!
 @property name
 @abstract The message name
 */
@property (nonatomic, readonly) NSString *name;

/*!
 @property source
 @abstract The source of the message (I2MSourceAccessory, I2MSourceAppleDevice, I2MSourceUnknown)
 */
@property (nonatomic, readonly) I2MSource source;

/*!
 @property parameterDefinitions
 @abstract An array of I2MParameterDefinitions (and subclasses) that represent this message's defined parameters
 */
@property (nonatomic, readonly) NSArray *parameterDefinitions;

/*!
 @method +messageDefinitionWithID:name:source:parameterDefinitions:
 @abstract Creates and returns a message definition.
 @param ID                   The message ID
 @param name                 The message name
 @param source               The source of the message (I2MSourceAccessory, I2MSourceAppleDevice, I2MSourceUnknown)
 @param parameterDefinitions An array of I2MParameterDefinitions
 @return The newly created and initialized message definition
 */
+ (instancetype)messageDefinitionWithID:(UInt16)ID
                                   name:(NSString *)name
                                 source:(I2MSource)source
                   parameterDefinitions:(NSArray *)parameterDefinitions;

/*!
 @method -initWithID:name:source:parameterDefinitions:
 @abstract Initializes a message definition
 @param ID                   The message ID
 @param name                 The message name
 @param source               The source of the message (I2MSourceAccessory, I2MSourceAppleDevice, I2MSourceUnknown)
 @param parameterDefinitions An array of I2MParameterDefinitions
 @return The initialized message definition
 */
- (instancetype)initWithID:(UInt16)ID
                      name:(NSString *)name
                    source:(I2MSource)source
      parameterDefinitions:(NSArray *)parameterDefinitions;

/*!
 @method    addAdditionalParameterDefinitions
 @abstract  Adds the specified parameter definitions to the message's current ones.
 @param     additionalParameterDefinitions
            The I2MParameterDefinitions to add.
 */
- (void)addAdditionalParameterDefinitions:(NSArray *)additionalParameterDefinitions;

@end
