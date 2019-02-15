//
//  ATSImportLogger.h
//  ATSMacApp
//
//  Created by Edgard Lima on 11/26/18.
//  Copyright (c) 2018 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#define ATS_LOG_LEVEL_FATAL   (1UL << 31UL)
#define ATS_LOG_LEVEL_ERROR   (1UL << 30UL)
#define ATS_LOG_LEVEL_WARNING (1UL << 29UL)
#define ATS_LOG_LEVEL_INFO    (1UL << 28UL)
#define ATS_LOG_LEVEL_TRACE   (1UL << 27UL)

#define ATS_LOG_LEVEL_MASK    0xF8000000

/**
 Used to determine the severity/level of the error. It maybe:
 ATS_LOG_LEVEL_FATAL - An error it can not recover from and the whole result might possible be discarded or aborted.
 ATS_LOG_LEVEL_ERROR - Something that is an error.
 ATS_LOG_LEVEL_WARNING - Something to pay attention to. Has a good chance of being an error, but may be not.
 ATS_LOG_LEVEL_INFO - Informational log
 ATS_LOG_LEVEL_TRACE - Very veborse trace
 */
#define ATS_LOG_LEVEL(errorCode)   ((errorCode) & ATS_LOG_LEVEL_MASK)

@interface ATSImportLogger : NSObject

/** If there are errors imporint, it will be set here any */
@property (readonly) NSArray<NSError *> *errors;

/** Has the number of log of each level. For instance, summary[@ATS_LOG_LEVEL_ERROR] returns the of log at ERROR level. */
@property (readonly) NSDictionary<NSNumber*, NSNumber*> *summary;

/** Add a new error to the Log. summary will be updated accordling. */
- (void)addError:(NSError*)error;

@end
