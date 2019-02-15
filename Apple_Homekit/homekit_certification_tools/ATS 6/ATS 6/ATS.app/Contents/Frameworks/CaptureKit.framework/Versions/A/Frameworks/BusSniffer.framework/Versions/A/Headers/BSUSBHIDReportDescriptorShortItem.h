//
//  BSUSBHIDReportDescriptorShortItem.h
//  BusSniffer
//
//  Created by Joe Basu on 11/9/12.
//  Copyright (c) 2012 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <BusSniffer/BSUSBHIDTypes.h>

/*!
 @class     BSUSBHIDReportDescriptorShortItem
 @abstract  A class that represents a short item in a USB HID report descriptor
 */

@interface BSUSBHIDReportDescriptorShortItem : NSObject

/*!
 @method    +usageDescriptionForHIDUsagePage:usage:
 @abstract  Provides a string descriptions for a HID usage.
 @param     usagePage
            The usage page.
 @param     usage
            The usage.
 */
+ (NSString *)usageDescriptionForHIDUsagePage:(UInt16)usagePage usage:(UInt16)usage;

/*!
 @method    +usagePageDescriptonForHIDUsagePage:
 @abstract  Provides a string descriptions for a HID usage page.
 @param     usagePage
            The usage page.
 */
+ (NSString *)usagePageDescriptonForHIDUsagePage:(UInt16)usagePage;

/*!
 @method +nameForHIDUsagePage:
 @abstract Provides a string description for a HID usage page
 @param usagePage The usage page
 @return The name of the usage page
 */
+ (NSString *)nameForHIDUsagePage:(UInt16)usagePage;

/*!
 @method +nameForGenericDesktopUsage:
 @abstract Provides a string name for a HID Generic Desktop usage
 @param usage The usage
 @return The name of the usage
 */
+ (NSString *)nameForGenericDesktopUsage:(UInt16)usage;

/*!
 @method +nameForConsumerUsage:
 @abstract Provides a string name for a HID Consumer usage
 @param usage The usage
 @return The name of the usage
 */
+ (NSString *)nameForConsumerUsage:(UInt16)usage;

/*!
 @method +nameForKeyboardUsage:
 @abstract Provides a string description for a HID Keyboard/Keypad usage
 @param usage The usage
 @return The name of the usage
 */
+ (NSString *)nameForKeyboardUsage:(UInt16)usage;

/*!
 @method +nameForLEDUsage:
 @abstract Provides a string description for a HID LED usage
 @param usage The usage
 @return The name of the usage
 */
+ (NSString *)nameForLEDUsage:(UInt16)usage;

/*!
 @method +nameForButtonUsage:
 @abstract Provides a string description for a HID Button usage
 @param usage The usage
 @return The name of the usage
 */
+ (NSString *)nameForButtonUsage:(UInt16)usage;

/*!
 @method +nameForTelephonyUsage:
 @abstract Provides a string description for a HID Telephony usage
 @param usage The usage
 @return The name of the usage
 */
+ (NSString *)nameForTelephonyUsage:(UInt16)usage;

/*!
 @property type
 @abstract The type of the item
 */
@property (nonatomic, readonly) BSUSBHIDReportDescriptorItemType type;

/*!
 @property tag
 @abstract The tag of the item
 */
@property (nonatomic, readonly) BSUSBHIDReportDescriptorItemTag tag;

/*!
 @property tag
 @abstract The value data associated with the item
 */
@property (nonatomic, readonly) NSData *data;

/*!
 @property usagePage
 @abstract The usage page that this item is associated with. Only relevant if the item is of type usage.
 */
@property (nonatomic, readonly) UInt16 usagePage;

/*!
 @property extendedUsage
 @abstract Returns YES if the usage was defined as "extended" (ie 4 bytes with both Usage Page and Usage).
           Only relevant if the item is of type usage.
 */
@property (nonatomic, readonly) BOOL extendedUsage;

/*!
 @property -signedValue
 @abstract Interprets the data property as a signed 32-bit integer
 @return The data represented as a signed 32-bit integer
 */
@property (nonatomic, readonly) SInt32 signedValue;

/*!
 @property -unsignedValue
 @abstract Interprets the data property as an unsigned 32-bit integer
 @return The data represented as an unsigned 32-bit integer
 */
@property (nonatomic, readonly) UInt32 unsignedValue;

/*!
 @method -initWithType:tag:data:usagePage:
 @abstract Initializes a new HID report descriptor item
 @param type      The item type
 @param tag       The item's tag
 @param data      Any data associated with item
 @param usagePage If the item is a usage, the usage page it belongs to, otherwise can be set to anything
 @return The initialized item
 */
- (id)initWithType:(BSUSBHIDReportDescriptorItemType)type tag:(BSUSBHIDReportDescriptorItemTag)tag data:(NSData *)data usagePage:(UInt16)usagePage extendedUsage:(BOOL)extendedUsage;


@end
