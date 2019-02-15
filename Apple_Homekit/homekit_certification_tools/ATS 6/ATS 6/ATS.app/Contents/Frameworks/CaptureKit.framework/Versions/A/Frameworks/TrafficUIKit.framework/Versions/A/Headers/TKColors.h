//
//  TKColors.h
//  TrafficKit
//
//  Created by Mark Hamlin on 5/28/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#define kTKRed [NSColor systemRedColor]
#define kTKGreen [NSColor systemGreenColor]
#define kTKBlue [NSColor systemBlueColor]
#define kTKYellow [NSColor systemYellowColor]
#define kTKOrange [NSColor systemOrangeColor]
#define kTKGray [NSColor systemGrayColor]

@interface TKColors : NSObject

/*!
 @method    setAuxTitleColor:
 @abstract  Sets the color for option content (ID X, typeX)
 */
+ (void)setAuxTitleColor:(NSColor *)color;
+ (NSColor * __strong *)auxTitleColorPointer;
#define TK_AUX_TITLE(str) TK_COLOR_POINTER([TKColors auxTitleColorPointer], TK_STRING(str))

/*!
 @method    setSourceAddressColor:
 @abstract  Sets the color for source addresses (mac, ip)
 */
+ (void)setSourceAddressColor:(NSColor *)color;
+ (NSColor * __strong *)sourceAddressColorPointer;
#define TK_SOURCE_ADDRESS(str) TK_COLOR_POINTER([TKColors sourceAddressColorPointer], TK_STRING(str))

/*!
 @method    setDestAddressColor:
 @abstract  Sets the color for destination addresses (mac, ip)
 */
+ (void)setDestAddressColor:(NSColor *)color;
+ (NSColor * __strong *)destAddressColorPointer;
#define TK_DEST_ADDRESS(str) TK_COLOR_POINTER([TKColors destAddressColorPointer], TK_STRING(str))

/*!
 @method    setSourcePortColor:
 @abstract  Sets the color for source ports (tcp, udp)
 */
+ (void)setSourcePortColor:(NSColor *)color;
+ (NSColor * __strong *)sourcePortColorPointer;
#define TK_SOURCE_PORT(port) TK_COLOR_POINTER([TKColors sourcePortColorPointer], TK_STRING_FMT(@"%u", port))

/*!
 @method    setDestPortColor:
 @abstract  Sets the color for dest ports (tcp, udp)
 */
+ (void)setDestPortColor:(NSColor *)color;
+ (NSColor * __strong *)destPortColorPointer;
#define TK_DEST_PORT(port) TK_COLOR_POINTER([TKColors destPortColorPointer], TK_STRING_FMT(@"%u", port))

/*!
 @method    setValueColor:
 @abstract  Sets the color for an arbitrary value
 */
+ (void)setValueColor:(NSColor *)color;
+ (NSColor * __strong *)valueColorPointer;
#define TK_VALUE_FMT(str, ...) TK_COLOR_POINTER([TKColors valueColorPointer], TK_STRING_FMT(str, ##__VA_ARGS__))
#define TK_VALUE(str) TK_COLOR_POINTER([TKColors valueColorPointer], TK_STRING(str))

/*!
 @method    setNullValueColor:
 @abstract  Sets the color for a null value
 */
+ (void)setNullValueColor:(NSColor *)color;
+ (NSColor * __strong *)nullValueColorPointer;
#define TK_NULL_VALUE_FMT() TK_COLOR_POINTER([TKColors nullValueColorPointer], TK_STRING(@"(null)"))

/*!
 @method    setKeyColor:
 @abstract  Sets the color for an arbitrary key
 */
+ (void)setKeyColor:(NSColor *)color;
+ (NSColor * __strong *)keyColorPointer;
#define TK_KEY(str) TK_COLOR_POINTER([TKColors keyColorPointer], TK_STRING(str))

#define TK_SOURCE_ADDR_PAIR(addr, port) TK_SOURCE_ADDRESS(addr), TK_STRING(@":"), TK_SOURCE_PORT(port)

#define TK_DEST_ADDR_PAIR(addr, port) TK_DEST_ADDRESS(addr), TK_STRING(@":"), TK_DEST_PORT(port)

#define TK_KEY_ATTRIBUTED_VALUE_PAIR(key, value) TK_KEY(key), TK_KEY(@": "), value

#define TK_KEY_VALUE_PAIR(key, value) TK_KEY(key), TK_STRING(@": "), TK_VALUE(value)

#define TK_KEY_ATTRIBUTED_VALUE_AUX_TITLE_TRIO(key, value, auxTitle) TK_KEY(key), TK_STRING(@": "), value, TK_STRING(@" "), TK_AUX_TITLE(auxTitle)

#define TK_KEY_VALUE_AUX_TITLE_TRIO(key, value, auxTitle) TK_KEY(key), TK_STRING(@": "), TK_VALUE(value), TK_STRING(@" "), TK_AUX_TITLE(auxTitle)

#define TK_KEY_AUX_TITLE_PAIR(key, auxTitle) TK_KEY(key), TK_STRING(@" "), TK_AUX_TITLE(auxTitle)

#define TK_KEY_NULL_VALUE_PAIR(key) TK_KEY(key), TK_STRING(@": "), TK_NULL_VALUE_FMT()

@end
