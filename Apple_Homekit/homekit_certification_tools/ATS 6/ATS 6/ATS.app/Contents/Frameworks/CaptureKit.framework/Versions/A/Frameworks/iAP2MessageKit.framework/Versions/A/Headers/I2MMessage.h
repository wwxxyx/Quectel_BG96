//
//  I2MMessage.h
//  iAP2MessageKit
//
//  Created by Joe Basu on 3/20/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <iAP2MessageKit/I2MTypes.h>
#import <iAP2MessageKit/I2MParameter.h>

NS_ASSUME_NONNULL_BEGIN

/*!
 @class     I2MMessage
 @abstract  A flexible record-style class representing an instance of an iAP2 message
 */

@interface I2MMessage : NSObject

/*!
 @property length
 @abstract The message length as defined in the Accessory Interface Specification
 @discussion This property is set, not computed, and may not reflect the actual amount of data available.
 */
@property (nonatomic) UInt16 length;

/*!
 @property identifier
 @abstract The message ID
 */
@property (nonatomic) UInt16 identifier;

/*!
 @property name
 @abstract The message name
 */
@property (nonatomic, copy) NSString *name;

/*!
 @property source
 @abstract The source of the message (I2MSourceAccessory, I2MSourceAppleDevice, I2MSourceUnknown)
 */
@property (nonatomic) I2MSource source;

/*!
 @property parameters
 @abstract An array of I2MParameters representing the child parameters of this message
 */
@property (nonatomic, copy) NSArray<I2MParameter *> *parameters;

/*!
 @property data
 @abstract The raw data for this message if it came over the wire
 */
@property (nonatomic, copy) NSData *data;

/*!
 The sequence number of the packet where the data came from
 */
@property (nonatomic) UInt8 packetSequenceNumber;

/*!
 @method +messageWithID:name:
 @abstract Creates and returns a new iAP2 message
 @param ID   The message ID
 @param name The message name. Can be nil if the name is not available.
 @return The newly created and initialized message
 */
+ (instancetype)messageWithID:(UInt16)ID name:(NSString *)name;

/*!
 @method -addParameter:
 @abstract Adds an I2MParameter to the parameters array
 @param parameter The parameter to be added
 */
- (void)addParameter:(I2MParameter *)parameter;

/*!
 @method -firstParameterForID:
 @abstract Returns the first parameter with the specified ID
 @param ID The ID of the desired parameter
 @return The desired parameter if found otherwise nil
 */
- (nullable I2MParameter *)firstParameterForID:(UInt16)ID;

/*!
 @method -parametersForID:
 @abstract Returns all parameters with the specified ID
 @param ID The ID of the desired parameter
 @return An array of the matching parameters
 */
- (nullable NSArray *)parametersForID:(UInt16)ID;

/*!
 @method -firstParameterForName:
 @abstract Returns the first parameter with the specified name
 @param name The name of the desired parameter
 @return The desired parameter if found otherwise nil
 */
- (nullable I2MParameter *)firstParameterForName:(NSString *)name;

/*!
 @method -parametersForName:
 @abstract Returns all parameters with the specified name
 @param name The name of the desired parameter
 @return An array of the matching parameters
 */
- (nullable NSArray *)parametersForName:(NSString *)name;

/*!
 @method -parametersForIDIndexPath:
 @abstract Returns all parameters with the specified index path.
 @param indexPath The index path to the desired parameter.
 @return An array of the matching parameters.
 */
- (nullable NSArray *)parametersForIDIndexPath:(NSIndexPath *)indexPath;

/*!
 @method -serialize:
 @abstract This method uses the ID and parameter properties to compose iAP2 message data suitable for sending over the wire
 @return iAP2 message data
 */
- (NSData *)serialize;

@end

NS_ASSUME_NONNULL_END
