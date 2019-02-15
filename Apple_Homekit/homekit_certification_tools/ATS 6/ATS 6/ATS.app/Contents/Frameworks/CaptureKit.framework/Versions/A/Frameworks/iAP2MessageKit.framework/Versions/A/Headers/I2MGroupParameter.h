//
//  I2MGroupParameter.h
//  iAP2MessageKit
//
//  Created by Joe Basu on 3/22/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <iAP2MessageKit/I2MParameter.h>

/*!
 @class     I2MGroupParameter
 @abstract  A flexible record-style class representing an instance of an iAP2 group parameter
 */

@interface I2MGroupParameter : I2MParameter

/*!
 @property subparameters
 @abstract  An array of I2MParameters representing the subparameters of this group parameter
 */
@property (nonatomic, copy) NSArray *subparameters;

/*!
 @method +groupParameterWithID:subparameters:name:
 @abstract Creates and returns a group parameter with the given ID, subparameters, and name
 @param ID            The parameter ID
 @param subparameters An array of I2MParameters representing the subparameters of this group parameter
 @param name          The parameter name. Can be nil if the name is not available.
 @return The newly created and initialized group parameter
 */
+ (id)groupParameterWithID:(UInt16)ID
             subparameters:(NSArray *)subparameters
                      name:(NSString *)name;

/*!
 @method +groupParameterWithID:name:
 @abstract Creates and returns a group parameter with the given ID and name
 @param ID            The parameter ID
 @param name          The parameter name. Can be nil if the name is not available.
 @return The newly created and initialized group parameter
 */
+ (id)groupParameterWithID:(UInt16)ID
                      name:(NSString *)name;

/*!
 @method -addSubparameter:
 @abstract Adds an I2MParameter to the subparameters array
 @param subparameter The subparameter to be added
 */
- (void)addSubparameter:(I2MParameter *)subparameter;

/*!
 @method -firstSubparameterForID:
 @abstract Returns the first subparameter with the specified ID
 @param ID The ID of the desired subparameter
 @return The desired subparameter if found otherwise nil
 */
- (I2MParameter *)firstSubparameterForID:(UInt16)ID;

/*!
 @method -subparametersForID:
 @abstract Returns all subparameters with the specified ID
 @param ID The ID of the desired subparameter
 @return An array of the matching subparameters
 */
- (NSArray *)subparametersForID:(UInt16)ID;

/*!
 @method -firstSubparameterForName:
 @abstract Returns the first subparameter with the specified name
 @param name The name of the desired subparameter
 @return The desired subparameter if found otherwise nil
 */
- (I2MParameter *)firstSubparameterForName:(NSString *)name;

/*!
 @method -subparametersForName:
 @abstract Returns all subparameters with the specified name
 @param name The name of the desired subparameter
 @return An array of the matching subparameters
 */
- (NSArray *)subparametersForName:(NSString *)name;

/*!
 @method -subparametersForIDIndexPath:
 @abstract Returns all subparameters with the specified index path.
 @param indexPath The index path to the desired subparameters.
 @return An array of the matching subparameters.
 */
- (NSArray *)subparametersForIDIndexPath:(NSIndexPath *)indexPath;

/*!
 @method -subparametersForIDIndexPath:indexLevel:
 @abstract Returns all subparameters with the specified index path starting
           from the given index level.
 @param indexPath The index path to the desired subparameters.
 @param indexLevel The starting indice of the index path.
 @return An array of the matching subparameters.
 */
- (NSArray *)subparametersForIDIndexPath:(NSIndexPath *)indexPath indexLevel:(NSUInteger)indexLevel;

/*!
 @method -serialize:
 @abstract This method uses the ID and data properties to compose iAP parameter data suitable for sending over the wire
 @return iAP parameter data
 */
- (NSData *)serialize;

@end
