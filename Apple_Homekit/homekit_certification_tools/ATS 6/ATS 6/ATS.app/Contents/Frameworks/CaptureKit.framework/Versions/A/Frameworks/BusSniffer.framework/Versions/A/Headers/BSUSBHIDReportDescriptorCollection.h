//
//  BSUSBHIDReportDescriptorCollection.h
//  BusSniffer
//
//  Created by Kip Nicol on 3/9/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSUSBHIDTypes.h>

@class BSUSBHIDReportFieldStructure;

/*!
 @interface BSUSBHIDReportDescriptorCollection
 @abstract  Represents a collection in a HID report descriptor.
 */
@interface BSUSBHIDReportDescriptorCollection : NSObject

/*!
 @property  type
 @abstract  The collection type (eg Application, Physical)
 */
@property (nonatomic, readonly, assign) BSUSBHIDReportDescriptorCollectionType collectionType;

/*!
 @property  parentCollection
 @abstract  The parent collection of the object.
 */
@property (nonatomic, readonly, assign) BSUSBHIDReportDescriptorCollection *parentCollection;

/*!
 @property  parentCollection
 @abstract  The root collection of the object.
 */
@property (nonatomic, readonly) BSUSBHIDReportDescriptorCollection *rootCollection;

/*!
 @property  childCollections
 @abstract  The child collections of the object.
 */
@property (nonatomic, readonly) NSArray *childCollections;

/*!
 @property  childCollections
 @abstract  The report field structures defined within the collection and its children.
 */
@property (nonatomic, readonly) NSArray *reportFieldStructures;

/*!
 @property  usages
 @abstract  The set of usages defined in the local scope at the time the collection was created.
 */
@property (nonatomic, readonly, copy) NSArray *usages;

/*!
 @method    -collectionWithAttributes:usages:
 @abstract  Creates and returns a collection with the given parameters.
 @param     type
            The type of the collection (eg Physical, Application, etc).
 @param     usages
            The set of usages defined in the local scope at the time the collection was created.
 */
+ (instancetype)collectionWithCollectionType:(BSUSBHIDReportDescriptorCollectionType)collectionType usages:(NSArray *)usages;

/*!
 @method    -addChildCollection:
 @abstract  Adds a child collection to the collection and sets the collection as the parent.
 */
- (void)addChildCollection:(BSUSBHIDReportDescriptorCollection *)childCollection;

/*!
 @method    -addReportFieldStructure:
 @abstract  Adds a report field structure to the collection.
 */
- (void)addReportFieldStructure:(BSUSBHIDReportFieldStructure *)reportFieldStructure;

/*!
 Method that search for the specified combination of usage and usagePage in the collection array of usages
 @param usage Usage identifier
 @param usage Usage Page identifier
 @return Returns YES if the usage was found. Otherwise, returns NO.
 */
- (BOOL)containsUsagePage:(UInt16)usagePage usage:(UInt16)usage;

@end
