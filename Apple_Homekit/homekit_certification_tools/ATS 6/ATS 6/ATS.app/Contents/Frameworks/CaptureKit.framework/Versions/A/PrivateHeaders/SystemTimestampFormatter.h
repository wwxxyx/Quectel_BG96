//
//  SystemTimestampFormatter.h
//  ATSMacApp
//
//  Created by Vinod Madigeri on 5/3/17.
//  Copyright © 2017 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SystemTimestampFormatter : NSFormatter

/**
 Method to make a System Timestamp Formatter with the input absolute timestamp (in Nanoseconds). Everything else (Locale, Timezone, Calendar) is picked from the system default.
 */
+ (instancetype)currentSystemTimestampFormatterWithAbsoluteTimestampSince1970:(UInt64)absoluteTimestampSince1970;

/**
 Method to retrive system timestamp information from the input dictionary.
 
 @note Pass along the output of dictionaryRepresentation method
 */
+ (instancetype)systemTimestampFormatterFromDictionary:(NSDictionary *)timestampInformationDictionary;

/**
 Method to get a dictionary representation of the formatter.
 
 @note Ideal to write to disk.
 */
@property (nonatomic, readonly) NSDictionary *dictionaryRepresentation;

/**
 Class property to get a default system timestamp format.
 */
@property (nonatomic, class, readonly) NSString *defaultSystemTimestampFormat;

/**
 Class property to get the current system timestamp format from NSUserDefaults
 */
@property (nonatomic, class, readonly) NSString *currentSystemTimestampFormat;

/**
 Utility method to get timestamp from now (current system time) to 1970.
 */
@property (nonatomic, class, readonly) UInt64 currentTimestampSince1970;

/**
 Method to notify the formatter of the user defaults change.
 */
- (void)dateFormatChanged;

/**
 Method to add the input timestamp to the stored date's timestamp since 1970 to get new absolute timestamp.
 
 @note Input and return units should be in NS.
 */
- (UInt64)timestampSince1970ByAddingTimestamp:(UInt64)timestamp;

@end
