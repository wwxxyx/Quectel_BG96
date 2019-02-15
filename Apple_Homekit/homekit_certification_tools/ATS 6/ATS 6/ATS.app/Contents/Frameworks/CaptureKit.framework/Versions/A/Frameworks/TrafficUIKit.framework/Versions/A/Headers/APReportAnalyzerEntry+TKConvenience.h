//
//  APReportAnalyzerEntry+TKConvenience.h
//  TrafficKit
//
//  Created by Mark Hamlin on 5/30/13.
//  Copyright (c) 2013 Apple Inc. All rights reserved.
//

#import "APReportAnalyzerEntry.h"

typedef enum {
    APPacketUnsignedIntegerField, // unsigned integer (decimal)
    APPacketHex1Field, // 1 hex char
    APPacketHex2Field, // 2 hex chars
    APPacketHex4Field, // 4 hex chars
    APPacketHex8Field, // 8 hex chars
    APPacketHexVariableLengthField, // 16 hex chars
    APPacketDoubleField, // double
    APPacketStringField, // String
    APPacketDataField, // NSData *
    APPacketBooleanField, // Boolean
} APPacketField;

@interface APReportAnalyzerEntry (TKConvenience)

/*!
 Sets the entries name
 @param name a NSString object value
 */
- (void)setProtocolLayerName:(NSString *)name;

/*!
 Sets the entries name
 @param name a TKAttributedString object value
 */
- (void)setProtocolLayerAttributedName:(TKAttributedString *)name;

/*!
 Sets a value with the display key and type
 @param value the object value
 @param key the key value with a string representation
 @param type an APPacketField type identifier
 @return Sets the entry with the new value of  key + value
 */
- (APReportAnalyzerEntry *)setValue:(id)value forKey:(NSString *)key type:(APPacketField)type;

/*!
 Adds a line with a formatted key and value
 @param key the key value with a string representation
 @param value the object value
 @return Adds a line with a formatted key + value
 */
- (APReportAnalyzerEntry *)addLineWithKey:(NSString *)key value:(id)value;

/*!
 Adds a line with a formatted key and value with the optional content value
 @param key the key value with a string representation
 @param value the object value
 @param auxTitle a auxiliary content value
 @return Adds a line with a formatted key + value + auxiliary content
 */
- (APReportAnalyzerEntry *)addLineWithKey:(NSString *)key value:(id)value andAuxTitle:(NSString *)auxTitle;

/*!
 Adds a line (child entry) formatted such that the index of the child is in the key. e.g. "Alerts[3]"
 The index is automatically set to based on the child's position in the children array.
 @param key The key value with a string representation
 @param value The object value
 @return Adds a line with a formatted key[index] + value
 */
- (APReportAnalyzerEntry *)addArrayElement:(NSString *)key value:(id)value;

/*!
 Sets the section title
 @param title the title value with a string representation
 */
- (void)setSectionTitle:(NSString *)title;

/*!
 Sets the section title with the auxiliary content
 @param title the title value with a string representation
 @param auxTitle a auxiliary content value
 */
- (void)setSectionTitle:(NSString *)title withAuxTitle:(NSString *)auxTitle;

/*!
 Sets the title of the entry (turns NSString into TK_ATTRIBUTED_STRING)
 @param label the title value with a string representation
 */
- (void)setTitleWithLabel:(NSString *)label;

/*!
 Sets the title of the entry (turns NSString into TK_ATTRIBUTED_STRING)
 @param label the title value with a string representation
 @param auxTitle a auxiliary content value
 */
- (void)setTitleWithLabel:(NSString *)label withAuxTitle:(NSString *)auxTitle;

/*!
 Adds an entry with the given data and label string
 @param data a NSData object
 @param label a string content
 @return Adds an entry with the given data and label string
 */
- (APReportAnalyzerEntry *)addEntryWithData:(NSData *)data label:(NSString *)label;

/*!
 Adds an entry with the given data, label string and auxiliary content
 @param data a NSData object
 @param label a string content
 @param auxTitle a auxiliary content value
 @return Adds an entry with the given data, label string + auxiliary content
 */
- (APReportAnalyzerEntry *)addEntryWithData:(NSData *)data label:(NSString *)label andAuxTitle:(NSString *)auxTitle;

/*!
 Sets a title in the form of "<label> (<quantity> <units>)"
 @param label a string content
 @param quantity represents the quantity with a unsigned long long
 @param units represents the unit of time with a string
 */
- (void)setTitleWithLabel:(NSString *)label quantity:(unsigned long long)quantity units:(NSString *)units;

/*!
 Sets a title in the form of "<label> (<quantity> <units>) (<ID X>, <typeX>)"
 @param label a string content
 @param quantity represents the quantity with a unsigned long long
 @param units represents the unit of time with a string
 @param auxTitle a auxiliary content value
 */
- (void)setTitleWithLabel:(NSString *)label quantity:(unsigned long long)quantity units:(NSString *)units withAuxTitle:(NSString *)auxTitle;

/*!
 Sets a title in the form of "<label> (<quantity> <units>)"
 @param label a string content
 @param color a NSColor object to set
 @param quantity represents the quantity with a unsigned long long
 @param units represents the unit of time with a string
 */
- (void)setTitleWithLabel:(NSString *)label color:(NSColor *)color quantity:(unsigned long long)quantity units:(NSString *)units;

/*!
 Sets formatted title with the error and the given label (prefix)
 @param label a string content
 @param error represents the error message with a string
 */
- (void)setTitleWithErrorLabel:(NSString *)label error:(NSString *)error;

/*!
 Adds a formatted line with the error and the given label (prefix)
 @param label a string content
 @param error represents the error message with a string
 */
- (void)addLineWithErrorLabel:(NSString *)label error:(NSString *)error;

/*!
 Adds a formatted line with the warning and the given label (prefix)
 @param label a string content
 @param error represents the error message with a string
 */
- (void)addLineWithWarningLabel:(NSString *)label error:(NSString *)error;

/*!
 Add key/value lines given an array of [(value, key), ...] pairs
 @param fields a NSArray of key/values
 */
- (void)addLinesWithValueKeyFields:(NSArray *)fields;

/*!
 Add key/value lines given an array of [(value, key), ...] pairs, inside of a section with the passed title
 @param fields a NSArray of key/values
 @param title a string content
 @return Adds an entry with the given NSArray fields
 */
- (APReportAnalyzerEntry *)addLinesWithValueKeyFields:(NSArray *)fields underSectionWithTitle:(NSString *)title;

/*!
 Add key/value lines given a dictionary of key strings and value strings
 @param fields a NSDictionary of key/values
 */
- (void)addLinesFromDictionary:(NSDictionary *)fields;

/*!
 Add a custom NSImageView of the represented data
 @param title a string content
 @param data a NSData object
 */
- (void)addImageViewWithTitle:(NSString *)title fromImageData:(NSData *)data;

@end
